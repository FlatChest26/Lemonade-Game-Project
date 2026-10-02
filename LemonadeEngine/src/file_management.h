#pragma once

#ifndef LEMONADE_ENGINE_SRC_FILE_MANAGEMENT_H
#define LEMONADE_ENGINE_SRC_FILE_MANAGEMENT_H

#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>

#if defined FILES_DEBUG || defined _DEBUG
#define FILES_COUT(...) COUT(__VA_ARGS__)

#else
#define FILES_COUT(...)

#endif // FILES_DEBUG || _DEBUG

using FilePath = std::filesystem::path;

namespace files
{
	inline bool filesystem_initialized = false;

	inline FilePath root_directory;

	inline FilePath data_directory;
	inline FilePath graphics_directory;
	inline FilePath config_directory;

#ifndef SNOWY__NO_RAWS_DIR
	inline FilePath raws_directory;
#endif

	inline bool validate_data_directory()
	{
		// Find Data Directory

		data_directory = root_directory / "data";

		if (!std::filesystem::exists(data_directory))
		{
			FILES_COUT("Error: Could not find the data directory.");
			return false;
		}

		// Validate Data Directory

		// Check data/config exists

		if (!std::filesystem::exists(data_directory / "config"))
		{
			FILES_COUT("Error: Data directory found but could not find a data/config directory.");
			return false;
		}
		else
		{
			config_directory = data_directory / "config";
		}

		// Check data/graphics exists

		if (!std::filesystem::exists(data_directory / "graphics"))
		{
			FILES_COUT("Error: Data directory found but could not find a data/graphics directory.");
			return false;
		}
		else
		{
			graphics_directory = data_directory / "graphics";
		}

		FILES_COUT("Found data directory: " << data_directory.generic_string());

		return true;
	}

#ifndef SNOWY__NO_RAWS_DIR
	inline bool validate_raws_directory()
	{
		// Find Raws Directory

		raws_directory = root_directory / "raws";

		if (!std::filesystem::exists(raws_directory))
		{
			FILES_COUT("Error: Could not find the raws directory.");
			return false;
		}

		if (!std::filesystem::exists(raws_directory / "core"))
		{
			FILES_COUT("Error: Raws directory found but could not find a raws/core directory.");
			return false;
		}

		if (!std::filesystem::exists(raws_directory / "mods"))
		{
			FILES_COUT("Error: Raws directory found but could not find a raws/mods directory.");
			return false;
		}

		FILES_COUT("Found raws directory: " << raws_directory.generic_string());

		return true;
	}
#endif

	inline bool validate_root_directory()
	{
		// Get Root Directory
		root_directory = FilePath{ "." };

		while (!std::filesystem::exists(root_directory / "data"))
		{
			root_directory /= "..";
			if (!std::filesystem::exists(root_directory))
			{
				FILES_COUT("Error: Could not find the data directory.");
				return false;
			}
		}

		return true;
	}
}

inline bool validate_filesystem()
{
	VALIDATE_OR_RET(files::validate_root_directory(), "Failed to validate root directory.");
	VALIDATE_OR_RET(files::validate_data_directory(), "Failed to validate data directory.");

#ifndef SNOWY__NO_RAWS_DIR
	VALIDATE_OR_RET(files::validate_raws_directory(), "Failed to validate raws directory.");
#endif

	files::filesystem_initialized = true;

	return true;
}

#endif // !LEMONADE_ENGINE_SRC_FILE_MANAGEMENT_H