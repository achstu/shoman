use crate::{movegen::GameStateWrapper, shobu::Player};

// black+, white-
pub fn eval(shobu_wrapper: &GameStateWrapper) -> i32 {
    let mut score: i32 = 0;

    // matrial
    const MATERIAL: i32 = 10000;
    for b in shobu_wrapper.shobu.boards {
        score += MATERIAL * b.bitboard[0].count_ones() as i32;
        score -= MATERIAL * b.bitboard[1].count_ones() as i32;
    }

    // mobility (avg 60 moves)
    const SHOVE: i32 = 5;
    const PASSIVE: i32 = 3;
    score += SHOVE * shobu_wrapper.count_shove(Player::Black) as i32;
    score += PASSIVE * shobu_wrapper.count_passive(Player::Black) as i32;

    score -= SHOVE * shobu_wrapper.count_shove(Player::White) as i32;
    score -= PASSIVE * shobu_wrapper.count_passive(Player::White) as i32;

    score
}
