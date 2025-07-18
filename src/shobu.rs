#![allow(dead_code)]

use {
    itertools::iproduct,
    once_cell::sync::Lazy,
    regex::Regex,
    strum::IntoEnumIterator,
    strum_macros::{Display, EnumIter, EnumString},
};

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Player {
    Black = 0,
    White = 1,
}

impl Player {
    pub fn other(&self) -> Self {
        match self {
            Player::Black => Player::White,
            Player::White => Player::Black,
        }
    }
}

#[derive(Display, EnumString, Copy, Clone, Debug, EnumIter)]
pub enum Direction {
    L = -1,
    R = 1,
    U = -4,
    D = 4,
    UR = -3,
    DL = 3,
    UL = -5,
    DR = 5,
}

#[derive(Copy, Clone, Debug)]
pub struct Vector {
    pub length: isize,
    pub direction: Direction,
}

impl Vector {
    pub fn to_string(&self) -> String {
        format!("{}{}", self.length, self.direction.to_string())
    }

    // for tests and dbg
    pub fn from_string(vector_string: &str) -> Option<Self> {
        let mut chars = vector_string.chars();
        Some(Vector {
            length: chars.next()?.to_digit(10)?.try_into().ok()?,
            direction: chars.as_str().parse::<Direction>().ok()?,
        })
    }

    pub fn all() -> impl Iterator<Item = Vector> {
        iproduct!(1..=2, Direction::iter()).map(|(length, direction)| Vector { length, direction })
    }
}

impl From<Vector> for isize {
    fn from(v: Vector) -> isize {
        v.length * (v.direction as isize)
    }
}

#[derive(Clone)]
pub struct Location {
    pub board_index: usize,
    pub stone_index: isize,
}

impl Location {
    pub fn new(board_index: usize, stone_index: isize) -> Self {
        Location {
            board_index,
            stone_index,
        }
    }
}

pub struct SubMove {
    pub vector: Vector,
    pub index: usize,
}

#[derive(Clone)]
pub struct Move {
    pub vector: Vector,
    pub passive: Location,
    pub aggressive: Location,
}

impl Move {
    pub fn from_string(move_string: &str) -> Option<Self> {
        static RE: Lazy<Regex> =
            Lazy::new(|| Regex::new(r"^(1|2)(U|UR|R|DR|D|DL|L|UL)(w|b)(\d+)(h|f)(\d+)$").unwrap());

        let caps = RE.captures(move_string)?;
        let (length, direction, p_board, p_index, a_board, a_index) = (
            caps.get(1)?.as_str().parse::<isize>().ok()?,
            caps.get(2)?.as_str().parse::<Direction>().ok()?,
            caps.get(3)?.as_str(),
            caps.get(4)?.as_str().parse::<isize>().ok()?,
            caps.get(5)?.as_str(),
            caps.get(6)?.as_str().parse::<isize>().ok()?,
        );

        if !Board::is_index_valid(p_index) || !Board::is_index_valid(a_index) {
            return None;
        }

        let p_board_index = if p_board == "b" { 0 } else { 1 };
        let a_board_index = p_board_index ^ if a_board == "h" { 1 } else { 3 };

        Some(Move {
            vector: Vector { length, direction },
            passive: Location {
                board_index: p_board_index,
                stone_index: p_index,
            },
            aggressive: Location {
                board_index: a_board_index,
                stone_index: a_index,
            },
        })
    }

    pub fn example() -> Self {
        Self::from_string("1Ub0h0").unwrap()
    }

    pub fn passive(&self) -> SubMove {
        SubMove {
            vector: self.vector,
            index: self.passive.stone_index as usize,
        }
    }
    pub fn aggressive(&self) -> SubMove {
        SubMove {
            vector: self.vector,
            index: self.aggressive.stone_index as usize,
        }
    }

    pub fn to_string(&self) -> String {
        format!(
            "{}{}{}{}{}",
            self.vector.to_string(),
            if self.passive.board_index == 0 {
                "b"
            } else {
                "w"
            },
            self.passive.stone_index,
            if self.aggressive.board_index > 1 {
                "f"
            } else {
                "h"
            },
            self.aggressive.stone_index
        )
    }
}

#[derive(Clone, Copy)]
pub struct Board {
    pub bitboard: [u16; 2],
}

impl Board {
    pub fn from_string(board_string: &str) -> Option<Self> {
        if board_string.len() != 16 {
            return None;
        }
        let mut bb_black = 0;
        let mut bb_white = 0;
        for (i, c) in board_string.chars().enumerate() {
            match c {
                'b' => bb_black |= 1 << i,
                'w' => bb_white |= 1 << i,
                '_' => (),
                _ => return None,
            };
        }
        Some(Board {
            bitboard: [bb_black, bb_white],
        })
    }

    const fn is_index_valid(i: isize) -> bool {
        0 <= i && i < 16
    }

    const fn is_move_valid(src: isize, dst: isize) -> bool {
        Self::is_index_valid(src)
            && Self::is_index_valid(dst)
            && ((src / 4 - dst / 4).abs() | (src % 4 - dst % 4).abs()) < 3
    }

    pub fn stone(&self, i: isize, player: Player) -> bool {
        self.bitboard[player as usize] & (1 << i) != 0
    }

    pub fn passive_submove(&self, sub_mv: &SubMove, player: Player) -> Option<Self> {
        let (src, vec) = (sub_mv.index as isize, sub_mv.vector);
        let mid = src + vec.direction as isize;
        let dst = src + vec.length * (vec.direction as isize);

        #[cfg(debug_assertions)]
        if !Self::is_index_valid(mid) || !Self::is_index_valid(dst) {
            return None;
        }

        let trace: u16 = (1 << mid) | (1 << dst);

        if !Self::is_move_valid(src, dst)
            || self.bitboard[player as usize] & (1 << src) == 0
            || trace & (self.bitboard[0] | self.bitboard[1]) != 0
        {
            return None;
        }

        let mut bb = self.bitboard;
        bb[player as usize] ^= (1 << src) | (1 << dst);
        Some(Board { bitboard: bb })
    }

    pub fn shove_submove(&self, sub_mv: &SubMove, player: Player) -> Option<Self> {
        let (src, vec) = (sub_mv.index as isize, sub_mv.vector);
        let mid = src + vec.direction as isize;
        let dst = src + vec.length * (vec.direction as isize);
        let nxt = src + (vec.length + 1) * (vec.direction as isize);

        #[cfg(debug_assertions)]
        if !Self::is_index_valid(dst) {
            return None;
        }

        let shove_bit = (Self::is_move_valid(dst, nxt) as u16)
            .checked_shl(nxt as u32)
            .unwrap_or(0);
        let trace: u16 = (1 << mid) | (1 << dst);
        let full_trace: u16 = trace | shove_bit;

        if !Self::is_move_valid(src, dst)
            || self.bitboard[player as usize] & (1 << src) == 0
            || (trace & self.bitboard[player.other() as usize]).count_ones() != 1
            || (full_trace & self.bitboard[player as usize]) != 0
        {
            return None;
        }

        let mut bb = self.bitboard;
        bb[player as usize] ^= (1 << src) | (1 << dst);
        bb[player.other() as usize] ^= (bb[player.other() as usize] & trace) | shove_bit;
        Some(Board { bitboard: bb })
    }

    pub fn aggressive_submove(&self, sub_mv: &SubMove, player: Player) -> Option<Self> {
        if let Some(board) = self.passive_submove(sub_mv, player) {
            return Some(board);
        }
        self.shove_submove(sub_mv, player)
    }

    pub fn is_terminal(&self) -> bool {
        self.bitboard[0] == 0 || self.bitboard[1] == 0
    }

    pub fn winner(&self) -> Option<Player> {
        match self.bitboard {
            [0, _] => Some(Player::White),
            [_, 0] => Some(Player::Black),
            _ => None,
        }
    }
}

#[derive(Clone)]
pub struct GameState {
    pub boards: [Board; 4],
}

impl GameState {
    pub fn from_string(game_string: &str) -> Option<Self> {
        let mut split = game_string.split(' ');
        let current_player = split.next()?;

        let shobu = GameState {
            boards: split
                .map(|s| Board::from_string(s))
                .collect::<Option<Vec<_>>>()?
                .try_into()
                .ok()?,
        };
        match current_player {
            "b" => (),
            "w" => (),
            _ => return None,
        };
        Some(shobu)
    }

    pub fn initial() -> Self {
        Self::from_string("b wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb")
            .unwrap()
    }

    pub fn make_move(&self, mv: &Move, player: Player) -> Option<Self> {
        let mut next = self.clone();

        next.boards[mv.passive.board_index] =
            self.boards[mv.passive.board_index].passive_submove(&mv.passive(), player)?;

        next.boards[mv.aggressive.board_index] =
            self.boards[mv.aggressive.board_index].aggressive_submove(&mv.aggressive(), player)?;

        Some(next)
    }

    pub fn is_terminal(&self) -> bool {
        self.boards.iter().any(|b| b.is_terminal())
    }

    pub fn winner(&self) -> Option<Player> {
        self.boards.iter().find_map(|b| b.winner())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn board_from_string() {
        assert!(Board::from_string("wwww________bbbb").is_some());
        assert!(Board::from_string("_bw_bw_bw__ww_b_").is_some());
        assert!(Board::from_string("bbbbbbbbbbbbbbbb").is_some());

        assert!(Board::from_string("_wbw___wbbb_ww_bbb_w_").is_none());
        assert!(Board::from_string("__bbw_bbbb__w").is_none());
        assert!(Board::from_string("__b_w_a_b_www_b_").is_none());
        assert!(Board::from_string("Hello World!").is_none());

        {
            let board = Board::from_string("wwww________bbbb").unwrap();
            assert_eq!(board.bitboard[0], 0b1111_0000_0000_0000);
            assert_eq!(board.bitboard[1], 0b0000_0000_0000_1111);
        }

        {
            let board = Board::from_string("_wbb__w_bbb_wb__").unwrap();
            assert_eq!(board.bitboard[0], 0b0010_0111_0000_1100);
            assert_eq!(board.bitboard[1], 0b0001_0000_0100_0010);
        }
    }

    #[rustfmt::skip]
    #[test]
    fn shobu_from_string() {
        assert!(GameState::from_string("b wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb").is_some());
        assert!(GameState::from_string("w wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb").is_some());

        assert!(GameState::from_string("").is_none());
        assert!(GameState::from_string("a wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb").is_none());
        assert!(GameState::from_string("ww_w___b____bbbb wwww________bbbb wwww________bbbb wwww________bbbb").is_none());
        assert!(GameState::from_string("b wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb").is_none());
        assert!(GameState::from_string("\t\n \tb wwww________bbbb wwww________bbbb wwww________bbbb wwww________bbbb\t  \n").is_none());
    }

    #[test]
    fn move_from_string() {
        assert!(Move::from_string("2URw12f7").is_some());
        assert!(Move::from_string("1Ub0h0").is_some());
        assert!(Move::from_string("2DRb15f6").is_some());
        assert!(Move::from_string("1Lw2h7").is_some());

        assert!(Move::from_string("3URw10f10").is_none());
        assert!(Move::from_string("2RUw10f10").is_none());
        assert!(Move::from_string("2Rw10f16").is_none());

        {
            let move_string = "2URb9f14";
            let mv = Move::from_string(move_string);
            assert!(mv.is_some());
            assert_eq!(move_string, mv.unwrap().to_string());
        }

        {
            let move_string = "1DLw9h15";
            let mv = Move::from_string(move_string);
            assert!(mv.is_some());
            assert_eq!(move_string, mv.unwrap().to_string());
        }
    }

    #[test]
    fn vector_from_string() {
        assert!(Vector::from_string("2L").is_some());
        assert!(Vector::from_string("2R").is_some());
        assert!(Vector::from_string("2U").is_some());
        assert!(Vector::from_string("2D").is_some());
        assert!(Vector::from_string("2UL").is_some());
        assert!(Vector::from_string("2UR").is_some());
        assert!(Vector::from_string("2DL").is_some());
        assert!(Vector::from_string("2DR").is_some());
        assert!(Vector::from_string("1L").is_some());
        assert!(Vector::from_string("1R").is_some());
        assert!(Vector::from_string("1U").is_some());
        assert!(Vector::from_string("1D").is_some());
        assert!(Vector::from_string("1UL").is_some());
        assert!(Vector::from_string("1UR").is_some());
        assert!(Vector::from_string("1DL").is_some());
        assert!(Vector::from_string("1DR").is_some());
    }

    #[rustfmt::skip]
    #[test]
    fn board_passive_push() {
        let vec = |s: &str| -> Vector { Vector::from_string(s).unwrap() };
        let sm = |i: isize, v: Vector| -> SubMove {
            SubMove {
                vector: v,
                index: i as usize,
            }
        };

        {
            let board = Board::from_string("wwww________bbbb").unwrap();

            let ok_vectors = [
                Vector::from_string("1U").unwrap(),
                Vector::from_string("2U").unwrap(),
                Vector::from_string("2UR").unwrap(),
                Vector::from_string("1UR").unwrap(),
            ];
            for vector in ok_vectors {
                assert!(board.passive_submove(&sm(13, vector), Player::Black).is_some());
                assert!(board.passive_submove(&sm(12, vector), Player::Black).is_some());
            }

            let bad_vectors = [
                Vector::from_string("1L").unwrap(),
                Vector::from_string("1R").unwrap(),
                Vector::from_string("2R").unwrap(),
                Vector::from_string("2L").unwrap(),
                Vector::from_string("1D").unwrap(),
                Vector::from_string("2D").unwrap(),
                Vector::from_string("1DL").unwrap(),
                Vector::from_string("2DL").unwrap(),
                Vector::from_string("1DR").unwrap(),
                Vector::from_string("2DR").unwrap(),
                Vector::from_string("2UL").unwrap(),
            ];
            for vector in bad_vectors {
                assert!(board.passive_submove(&sm(13, vector), Player::Black).is_none());
                assert!(board.passive_submove(&sm(0, vector), Player::Black).is_none());
                assert!(board.passive_submove(&sm(7, vector), Player::Black).is_none());
                assert!(board.passive_submove(&sm(12, vector), Player::Black).is_none());
            }
        }

        {
            let board = Board::from_string("__wb_bw_w__w_b__").unwrap();
            assert!(board.passive_submove(&sm(3, vec("1D")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(5, vec("1D")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(5, vec("1DR")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(5, vec("2DR")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(5, vec("1L")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(5, vec("1U")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(5, vec("1UL")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(13, vec("1L")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(13, vec("1U")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(13, vec("1UR")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(13, vec("2UR")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(13, vec("1R")), Player::Black).is_some());
            assert!(board.passive_submove(&sm(13, vec("2R")), Player::Black).is_some());

            // wrong vector
            assert!(board.passive_submove(&sm(13, vec("2L")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("1UL")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("2UL")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("2U")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("1D")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("2D")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("1DL")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("2DL")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("1DR")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(13, vec("2DR")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(5, vec("1R")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(5, vec("1UR")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(5, vec("1DL")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(5, vec("2R")), Player::Black).is_none());

            // trying to push white
            assert!(board.passive_submove(&sm(6, vec("2D")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(11, vec("2L")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(11, vec("1U")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(11, vec("1DL")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(11, vec("1UL")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(2, vec("1U")), Player::Black).is_none());

            // trying to push empty tile
            assert!(board.passive_submove(&sm(9, vec("1R")), Player::Black).is_none());
            assert!(board.passive_submove(&sm(15, vec("2L")), Player::Black).is_none());
        }
    }

    #[rustfmt::skip]
    #[test]
    fn board_aggressive_push() {
        let vec = |s: &str| -> Vector { Vector::from_string(s).unwrap() };
        let sm = |i: isize, v: Vector| -> SubMove {
            SubMove {
                vector: v,
                index: i as usize,
            }
        };

        {
            let board = Board::from_string("__w_b_bww______b").unwrap();
            // passive
            assert!(board.aggressive_submove(&sm(4, vec("1U")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(4, vec("1UR")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(4, vec("1R")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(4, vec("1DR")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(4, vec("2DR")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(6, vec("1UR")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(6, vec("2DL")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(15, vec("1U")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(15, vec("2L")), Player::Black).is_some());

            // push
            assert!(board.aggressive_submove(&sm(15, vec("2U")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(6, vec("1U")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(6, vec("1R")), Player::Black).is_some());
            assert!(board.aggressive_submove(&sm(4, vec("2D")), Player::Black).is_some());
        }

        {
            let board = Board::from_string("___w_____w__b___").unwrap();
            let sub_mv = &sm(12, vec("2UR"));
            assert!(board.aggressive_submove(sub_mv, Player::Black).is_none());
        }
        {
            let board = Board::from_string("_________w__b___").unwrap();
            let sub_mv = &sm(12, vec("2UR"));
            assert!(board.aggressive_submove(sub_mv, Player::Black).is_some());
        }
        {
            let board = Board::from_string("______w__w__b___").unwrap();
            let sub_mv = &sm(12, vec("2UR"));
            assert!(board.aggressive_submove(sub_mv, Player::Black).is_none());
        }
        {
            let board = Board::from_string("___b_____w__b___").unwrap();
            let sub_mv = &sm(12, vec("2UR"));
            assert!(board.aggressive_submove(sub_mv, Player::Black).is_none());
        }
        {
            let board = Board::from_string("______w__b______").unwrap();
            let sub_mv = &sm(9, vec("2UR"));
            assert!(board.aggressive_submove(sub_mv, Player::Black).is_some());
        }
        {
            let board = Board::from_string("___w__b_________").unwrap();
            let sub_mv = &sm(6, vec("2UR"));
            assert!(board.aggressive_submove(sub_mv, Player::Black).is_none());
        }
        {
            let board = Board::from_string("___w__b_________").unwrap();
            let sub_mv = &sm(6, vec("1UR"));
            assert!(board.aggressive_submove(sub_mv, Player::Black).is_some());
        }
    }

    #[test]
    fn shobu_make_move() {
        {
            let shobu = GameState::initial();
            let mv = Move::from_string("1URb13h12").unwrap();

            assert!(shobu.make_move(&mv, Player::Black).is_some());
        }
    }
}
