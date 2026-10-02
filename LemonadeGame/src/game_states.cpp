#include "game_states.h"
#include "game.h"

GameSettings& GameState::get_game_settings() const
{
	return m_game->get_settings();
}

void GameState::render() const
{
}