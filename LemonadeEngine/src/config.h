#pragma once

#ifndef LEMONADE_ENGINE_SRC_CONFIG_H
#define LEMONADE_ENGINE_SRC_CONFIG_H

#include "file_management.h"
#include <fstream>
#include <libtcod.hpp>

#if defined CONFIG_DEBUG || defined _DEBUG
#define CONFIG_COUT(...) COUT(__VA_ARGS__)

#else
#define CONFIG_COUT(...)

#endif // CONFIG_DEBUG || DEBUG

namespace cfg
{
	inline FilePath settings_path;

	namespace settings
	{
		inline std::string RENDERER = "SDL";

		inline int CONSOLE_WIDTH = 96, CONSOLE_HEIGHT = 24;
		inline int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 800;

		inline bool FULLSCREEN = false;
		inline int FULLSCREEN_WIDTH = 1280, FULLSCREEN_HEIGHT = 1024;

		inline std::string TILESET = "data/graphics/(8x12) curses_640x300.png";

		inline bool SHOW_FPS = false;
		inline bool USE_CONSOLE_SIZE = true;
	}

	inline const char* REQUIRED_INIT_KEYS[] =
	{
		"renderer",

		"screen_width",
		"screen_height",

		"tileset",

		"fullscreen",
		"fullscreen_width",
		"fullscreen_height",
	};

	inline bool parse_settings_cfg(const char* filename)
	{
		TCODParser parser{};
		auto init_struct = parser.newStructure("init");

		init_struct->addProperty("renderer", TCOD_TYPE_STRING, true);
		init_struct->addProperty("console_width", TCOD_TYPE_INT, false);
		init_struct->addProperty("console_height", TCOD_TYPE_INT, false);
		init_struct->addProperty("screen_width", TCOD_TYPE_INT, true);
		init_struct->addProperty("screen_height", TCOD_TYPE_INT, true);
		init_struct->addProperty("use_console_size", TCOD_TYPE_BOOL, true);
		init_struct->addProperty("tileset", TCOD_TYPE_STRING, true);
		init_struct->addProperty("fullscreen", TCOD_TYPE_BOOL, true);
		init_struct->addProperty("fullscreen_width", TCOD_TYPE_INT, true);
		init_struct->addProperty("fullscreen_height", TCOD_TYPE_INT, true);

		init_struct->addProperty("show_fps", TCOD_TYPE_BOOL, false);

		parser.run(filename, NULL);

		// Checking Keys Keys

		for (const char* required_key : REQUIRED_INIT_KEYS)
		{
			if (!parser.hasProperty(((std::string)"init." + required_key).c_str()))
			{
				CONFIG_COUT((std::string)"Error: Parsing settings failed. No 'init." + required_key + "' key found in settings.cfg.");
				return false;
			}
		}

		// Console Resolution
		settings::CONSOLE_WIDTH = parser.getIntProperty("init.console_width");
		settings::CONSOLE_HEIGHT = parser.getIntProperty("init.console_height");

		CONFIG_COUT("CONSOLE_WIDTH: " << settings::CONSOLE_WIDTH);
		CONFIG_COUT("CONSOLE_HEIGHT: " << settings::CONSOLE_HEIGHT);

		// Screen Resolution
		settings::SCREEN_WIDTH = parser.getIntProperty("init.screen_width");
		settings::SCREEN_HEIGHT = parser.getIntProperty("init.screen_height");

		CONFIG_COUT("SCREEN_WIDTH: " << settings::SCREEN_WIDTH);
		CONFIG_COUT("SCREEN_HEIGHT: " << settings::SCREEN_HEIGHT);

		// Use Console Size
		settings::USE_CONSOLE_SIZE = parser.getBoolProperty("init.use_console_size");

		CONFIG_COUT("USE_CONSOLE_SIZE: " << (settings::USE_CONSOLE_SIZE ? "true" : "false"));

		// Tileset
		settings::TILESET = parser.getStringProperty("init.tileset");

		CONFIG_COUT("TILESET: " << settings::TILESET);

		// Renderer
		settings::RENDERER = parser.getStringProperty("init.renderer");

		CONFIG_COUT("RENDERER: " << settings::RENDERER);

		// Fullscreen
		settings::FULLSCREEN = parser.getBoolProperty("init.fullscreen");
		settings::FULLSCREEN_WIDTH = parser.getIntProperty("init.fullscreen_width");
		settings::FULLSCREEN_HEIGHT = parser.getIntProperty("init.fullscreen_height");

		CONFIG_COUT("FULLSCREEN: " << (settings::FULLSCREEN ? "true" : "false"));
		CONFIG_COUT("FULLSCREEN_WIDTH: " << settings::FULLSCREEN_WIDTH);
		CONFIG_COUT("FULLSCREEN_HEIGHT: " << settings::FULLSCREEN_HEIGHT);

		// Settings
		settings::SHOW_FPS = parser.getBoolProperty("init.show_fps");

		CONFIG_COUT("SHOW_FPS: " << (settings::SHOW_FPS ? "true" : "false"));

		return true;
	}

	inline bool load_settings_file()
	{
		// Find config/settings.cfg
		settings_path = files::config_directory / "settings.cfg";

		if (!std::filesystem::exists(settings_path)) // Failed to find config/settings.cfg
		{
			CONFIG_COUT("Error: Could not find settings.cfg file " + settings_path.generic_string());
			return false;
		}
		// Open config/settings.cfg
		auto parse_result = parse_settings_cfg("data/config/settings.cfg");

		return true;
	}

	inline bool init()
	{
		VALIDATE_OR_RET(load_settings_file(), "Failed to load settings file.");

		return true;
	}
}

inline bool config_init()
{
	VALIDATE_OR_RET(cfg::init(), "Failed to initialize.");

	return true;
}

#endif // !LEMONADE_ENGINE_SRC_CONFIG_H