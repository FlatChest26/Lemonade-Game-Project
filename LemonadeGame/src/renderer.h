#pragma once

#ifndef LEMONADE_GAME_SRC_RENDERER_H
#define LEMONADE_GAME_SRC_RENDERER_H

struct RenderParams;

class GameState;
class WorldGameState;
class World;
class Camera;
class Entity;
class TileMap;

class Renderer
{
protected:
	GameState* m_game_state;

public:
	Renderer(GameState* game_state = nullptr);

	// -- Getters -- //
	GameState* get_game_state() const;
	WorldGameState* get_world_game_state() const;
	World* get_world() const;
	Camera* get_camera() const;
	Entity* get_player() const;
	TileMap* get_tile_map() const;

public:
	// -- Utilities -- //

	virtual void render(RenderParams params) const;

protected:
	// -- Render Functions -- //
	virtual void render_entities(RenderParams params) const;
	virtual void render_entity(RenderParams params, const Entity* entity) const;

	virtual void render_tile_map(RenderParams params) const;
};

#endif // !LEMONADE_GAME_SRC_RENDERER_H
