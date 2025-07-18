use std::cmp::{max, min};

use crate::{
    evaluation,
    movegen::{self, GameStateWrapper},
    shobu::{Move, Player},
};

impl Player {
    fn best_score(&self) -> i32 {
        match self {
            Player::Black => i32::MAX,
            Player::White => i32::MIN,
        }
    }

    fn worst_score(&self) -> i32 {
        self.other().best_score()
    }
}

fn minimax_rec(
    shobu_wrapper: &GameStateWrapper,
    player: Player,
    depth: usize,
    cnt: &mut usize,
) -> (i32, Option<Move>) {
    *cnt += 1;

    if depth == 0 {
        let eval = evaluation::eval(shobu_wrapper);
        return (eval, None);
    }

    let moves = movegen::all_moves(shobu_wrapper, player);
    if moves.is_empty() {
        return (player.worst_score(), None);
    }

    if let Some(winner) = shobu_wrapper.shobu.winner() {
        return (
            if winner == player {
                player.best_score()
            } else {
                player.worst_score()
            },
            None,
        );
    }

    let mut results = vec![];

    for mv in moves {
        let mut child = shobu_wrapper.clone();
        child.update(&mv, player);
        let (eval, _) = minimax_rec(&child, player.other(), depth - 1, cnt);
        results.push((eval, Some(mv)));
    }

    match player {
        Player::Black => results.into_iter().max_by_key(|&(eval, _)| eval).unwrap(),
        Player::White => results.into_iter().min_by_key(|&(eval, _)| eval).unwrap(),
    }
    // None
}

pub fn minimax(shobu_wrapper: &GameStateWrapper, player: Player) -> Option<Move> {
    let mut cnt = 0;
    let (_, mv) = minimax_rec(shobu_wrapper, player, 3, &mut cnt);
    println!("minimax: {}", cnt);
    mv
}

fn alphabeta_rec(
    shobu_wrapper: &GameStateWrapper,
    player: Player,
    ab: (i32, i32),
    depth: usize,
    cnt: &mut usize,
) -> (i32, Option<Move>) {
    *cnt += 1;

    let (mut alpha, mut beta) = ab;

    if depth == 0 {
        let eval = evaluation::eval(shobu_wrapper);
        return (eval, None);
    }

    let moves = movegen::all_moves(shobu_wrapper, player);
    if moves.is_empty() {
        return (player.worst_score(), None);
    }

    if let Some(winner) = shobu_wrapper.shobu.winner() {
        return (
            if winner == player {
                player.best_score()
            } else {
                player.worst_score()
            },
            None,
        );
    }

    let mut results = vec![];

    match player {
        Player::Black => {
            for mv in moves {
                let mut child = shobu_wrapper.clone();
                child.update(&mv, player);
                let (eval, _) =
                    alphabeta_rec(&child, player.other(), (alpha, beta), depth - 1, cnt);
                results.push((eval, Some(mv)));

                alpha = max(alpha, eval);
                if beta <= alpha {
                    break;
                }
            }

            results.into_iter().max_by_key(|&(eval, _)| eval).unwrap()
        }
        Player::White => {
            for mv in moves {
                let mut child = shobu_wrapper.clone();
                child.update(&mv, player);
                let (eval, _) =
                    alphabeta_rec(&child, player.other(), (alpha, beta), depth - 1, cnt);
                results.push((eval, Some(mv)));

                beta = min(beta, eval);
                if beta <= alpha {
                    break;
                }
            }
            results.into_iter().min_by_key(|&(eval, _)| eval).unwrap()
        }
    }
}

pub fn alphabeta(shobu_wrapper: &GameStateWrapper, player: Player) -> Option<Move> {
    let mut cnt = 0;
    let (_, mv) = alphabeta_rec(shobu_wrapper, player, (i32::MIN, i32::MAX), 3, &mut cnt);
    // println!("alphabeta: {}", cnt);
    mv
}
