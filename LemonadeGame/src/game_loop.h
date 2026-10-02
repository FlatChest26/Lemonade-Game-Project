#pragma once

#ifndef LEMONADE_GAME_SRC_GAME_LOOP_H
#define LEMONADE_GAME_SRC_GAME_LOOP_H

void new_game();

void render();
void update();
void handle_events();

bool run();

#endif // !LEMONADE_GAME_SRC_GAME_LOOP_H