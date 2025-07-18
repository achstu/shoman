#![allow(dead_code)]

use indicatif::{ProgressBar, ProgressStyle};
use movegen::GameStateWrapper;
use shobu::{GameState, Player};

mod evaluation;
mod movegen;
mod search;
mod shobu;

struct GameResult {
    pub turns: usize,
    pub winner: Player,
    pub reason: String,
}

// 95% games ends within this turn mark
pub const CUTOFF: usize = 350;

fn random_game() -> Option<GameResult> {
    let mut shobu_wrapper = GameStateWrapper::new(&GameState::initial());
    let mut current_player = Player::Black;

    let mut turns = 0;

    while !shobu_wrapper.shobu.is_terminal() {
        if let Some(mv) = movegen::random_move(&shobu_wrapper, current_player) {
            shobu_wrapper.update(&mv, current_player);
            current_player = current_player.other();
        } else {
            return Some(GameResult {
                turns,
                winner: current_player.other(),
                reason: String::from("no valid moves"),
            });
        }

        turns += 1;

        if turns == CUTOFF {
            return None;
        }
    }

    Some(GameResult {
        turns,
        winner: current_player.other(),
        reason: String::from("all stones pushed"),
    })
}

fn start_simulation() {
    const GAMES: usize = 50000;
    let bar = ProgressBar::new(GAMES as u64);

    bar.set_style(
        ProgressStyle::default_bar()
            .template("{spinner:.green} [{elapsed_precise}] [{bar:40.cyan/blue}] {pos}/{len} ({per_sec}, {eta})")
            .unwrap()
            .progress_chars("=> "),
    );

    let mut black_wins = 0;
    let mut valid_games = 0;
    let mut no_moves = 0;
    let mut no_moves_black = 0;

    for _ in 0..GAMES {
        if let Some(result) = random_game() {
            let z = match result.winner {
                Player::Black => 1,
                Player::White => 0,
            };
            black_wins += z;
            valid_games += 1;

            match result.reason.as_str() {
                "no valid moves" => {
                    no_moves += 1;
                    no_moves_black += z;
                }
                _ => (),
            }
        }
        bar.inc(1);
    }
    bar.finish();

    println!("valid games: {}", valid_games as f32 / GAMES as f32);
    println!("no move end: {}", no_moves as f32 / valid_games as f32);
    println!(
        "no move black winrate: {}",
        no_moves_black as f32 / no_moves as f32
    );
    println!("black winrate: {}", black_wins as f32 / valid_games as f32);
}

fn alphabeta_play() {
    let shobu = GameState::initial();
    let mut sw = GameStateWrapper::new(&shobu);
    let mut current_player = Player::Black;

    while let Some(mv) = search::alphabeta(&sw, current_player) {
        println!("{:?} => {}", current_player, mv.to_string());
        sw.update(&mv, current_player);
        current_player = current_player.other();
    }
}

use std::io;

fn main() {
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read line");

    let shobu = GameState::from_string(&input.trim()).unwrap();
    let sw = GameStateWrapper::new(&shobu);
    let mv = search::alphabeta(
        &sw,
        if input.chars().next().unwrap() == 'b' {
            Player::Black
        } else {
            Player::White
        },
    )
    .unwrap();

    println!("{}", mv.to_string());
}
