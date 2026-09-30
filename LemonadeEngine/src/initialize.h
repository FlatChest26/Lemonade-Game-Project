#pragma once

#ifndef LEMONADE_ENGINE_SRC_INITIALIZE_H
#define LEMONADE_ENGINE_SRC_INITIALIZE_H

#include "file_management.h"
#include "config.h"
#include "output.h"

inline bool initialize( int argc, const char* argv[] )
{
	if ( !validate_filesystem() )
		return false;

	if ( !config_init() )
		return false;

	SDL_LogSetAllPriority( SDL_LOG_PRIORITY_WARN );
	atexit( TCOD_quit );

	if ( !output::setup_context( argc, argv ) )
		return false;

	return true;
}

#endif // !LEMONADE_ENGINE_SRC_INITIALIZE_H