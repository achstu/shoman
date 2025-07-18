#![allow(dead_code)]

use crate::shobu::{Board, GameState, Location, Move, Player, SubMove, Vector};
use itertools::iproduct;

#[derive(Debug, Clone)]
struct BoardWrapper {
    passive: [u16; 21],
    shove: [u16; 21],
}

impl BoardWrapper {
    const SHIFT: isize = 10;

    pub fn new(board: &Board, player: Player) -> Self {
        let mut result = BoardWrapper {
            passive: Default::default(),
            shove: Default::default(),
        };

        for vector in Vector::all() {
            for i in (0isize..16).filter(|&i| board.stone(i, player)) {
                let sub_mv = SubMove {
                    index: i as usize,
                    vector,
                };
                if board.shove_submove(&sub_mv, player).is_some() {
                    result.shove[(Self::SHIFT + isize::from(vector)) as usize] |= 1 << i;
                } else if board.passive_submove(&sub_mv, player).is_some() {
                    result.passive[(Self::SHIFT + isize::from(vector)) as usize] |= 1 << i;
                }
            }
        }
        result
    }

    pub fn get_passive(&self, vector: Vector) -> u16 {
        self.passive[(Self::SHIFT + isize::from(vector)) as usize]
    }

    pub fn get_shove(&self, vector: Vector) -> u16 {
        self.shove[(Self::SHIFT + isize::from(vector)) as usize]
    }

    pub fn get_aggressive(&self, vector: Vector) -> u16 {
        self.get_passive(vector) | self.get_shove(vector)
    }
}

#[derive(Clone)]
pub struct GameStateWrapper {
    pub shobu: GameState,
    boards: [[BoardWrapper; 4]; 2],
}

impl GameStateWrapper {
    pub fn new(shobu: &GameState) -> Self {
        GameStateWrapper {
            shobu: shobu.clone(),
            boards: [
                std::array::from_fn(|i| BoardWrapper::new(&shobu.boards[i], Player::Black)),
                std::array::from_fn(|i| BoardWrapper::new(&shobu.boards[i], Player::White)),
            ],
        }
    }

    pub fn update(&mut self, mv: &Move, player: Player) {
        // println!("{}", mv.to_string());
        self.shobu = self.shobu.make_move(mv, player).unwrap();

        for p in [Player::Black, Player::White] {
            self.boards[p as usize][mv.passive.board_index] =
                BoardWrapper::new(&self.shobu.boards[mv.passive.board_index], p);

            self.boards[p as usize][mv.aggressive.board_index] =
                BoardWrapper::new(&self.shobu.boards[mv.aggressive.board_index], p);
        }
    }

    pub fn count_shove(&self, player: Player) -> usize {
        let mut res = 0;
        for bw in self.boards[player as usize].iter() {
            res += bw
                .shove
                .iter()
                .map(|mask| mask.count_ones() as usize)
                .sum::<usize>();
        }
        res
    }
    pub fn count_passive(&self, player: Player) -> usize {
        let mut res = 0;
        for bw in self.boards[player as usize].iter() {
            res += bw
                .passive
                .iter()
                .map(|mask| mask.count_ones() as usize)
                .sum::<usize>();
        }
        res
    }
}

pub fn all_moves(shobu_wrapper: &GameStateWrapper, player: Player) -> Vec<Move> {
    let mut result = vec![];

    for vector in Vector::all() {
        enum Mode {
            Passive,
            Shove,
            Aggressive,
        }
        let mut add_moves = |passive_board: usize, aggressive_board: usize, mode: Mode| {
            let from_mask = |mask: u16| (0isize..16).filter(move |i| (1 << i) & mask != 0);

            for (passive_index, aggressive_index) in iproduct!(
                from_mask(shobu_wrapper.boards[player as usize][passive_board].get_passive(vector)),
                from_mask(match mode {
                    Mode::Passive =>
                        shobu_wrapper.boards[player as usize][aggressive_board].get_passive(vector),
                    Mode::Shove =>
                        shobu_wrapper.boards[player as usize][aggressive_board].get_shove(vector),
                    Mode::Aggressive => shobu_wrapper.boards[player as usize][aggressive_board]
                        .get_aggressive(vector),
                })
            ) {
                let mv = Move {
                    vector,
                    passive: Location::new(passive_board, passive_index),
                    aggressive: Location::new(aggressive_board, aggressive_index),
                };
                result.push(mv);
            }
        };

        add_moves(0, 3, Mode::Aggressive);
        add_moves(1, 2, Mode::Aggressive);
        add_moves(0, 1, Mode::Passive);
        add_moves(0, 1, Mode::Shove);
        add_moves(1, 0, Mode::Shove);
    }
    result
}

use rand::prelude::*;

pub fn random_move(shobu_wrapper: &GameStateWrapper, player: Player) -> Option<Move> {
    let mut rng = rand::rng();
    let moves = all_moves(shobu_wrapper, player);
    let mv = moves.choose(&mut rng).cloned();
    mv
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::shobu::Direction;

    #[test]
    fn moves_initial() {
        {
            let board = Board::from_string("wwww________bbbb").unwrap();
            let sub_mv = SubMove {
                vector: Vector {
                    length: 2,
                    direction: Direction::UR,
                },
                index: 12,
            };
            assert!(board.passive_submove(&sub_mv, Player::Black).is_some());
        }
    }

    #[test]
    fn shove_board_wrapper() {
        let board = Board::from_string("wwww________bbbb").unwrap();
        let board_wrapper = BoardWrapper::new(&board, Player::Black);
        println!("{:?}", board_wrapper);
        let from_mask = |mask: u16| (0isize..16).filter(move |i| (1 << i) & mask != 0);

        let vector = Vector::from_string("2UL").unwrap();
        println!(
            "{:?}",
            from_mask(board_wrapper.get_passive(vector)).collect::<Vec<_>>()
        );

        let vector = Vector::from_string("2UL").unwrap();
        println!(
            "{:?}",
            from_mask(board_wrapper.get_shove(vector)).collect::<Vec<_>>()
        );
    }
}
