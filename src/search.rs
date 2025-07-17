use crate::{
    movegen::{all_moves, GameStateWrapper},
    shobu::Player,
};

pub fn minimax(shobu_wrapper: &GameStateWrapper) {
    for _mv in all_moves(shobu_wrapper, Player::Black) {
        todo!();
    }
}
