/*
 * Copyright (C) 2015 Alex Rathbone
 *
 * This file is part of Meteorite.
  
 * Meteorite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
  
 * Meteorite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
  
 * You should have received a copy of the GNU General Public License
 * along with Meteorite.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <boost/shared_ptr.hpp>

namespace Meteorite
{
	struct LoggerSettings
	{
		/* If set to true, 'filePath' will be opened as a file and log messages will be written there. */
		bool outputToFile = false;

		/* Must be set if outputToFile is true. */
		std::string filePath;

		/* If set to true, all log messages will be passed to std::cout. */
		bool outputToStdOut = false;

		/* If set to true, all log messages will be prefixed with: [YYYY-MM-DD HH:mm:SS] */
		bool enableTimestamps = false;
	};

	enum LogLevel
	{
		LOG_LEVEL_NONE, // no messages will be displayed!
		LOG_LEVEL_FATAL,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_INFORMATION,
		LOG_LEVEL_DEBUG,
	};

	class Logger
	{
	public:
		Logger(const LoggerSettings& settings);
		~Logger();

		/* Attempts to initialise the logger, returns false on any error. */
		bool initialise();

		/* Sets the level that the logger will print at. */
		void setLevel(const LogLevel level);

		void fatal(const std::string& message);
		void error(const std::string& message);
		void warning(const std::string& message);
		void info(const std::string& message);
		void debug(const std::string& message);

		/* Outputs a line break with no timestamp or tag. */
		void lineBreak(const LogLevel level = LOG_LEVEL_NONE);

	private:
		LoggerSettings settings;
		bool initialised = false;
		LogLevel currentLevel = LOG_LEVEL_DEBUG;
		std::ofstream logFile;

		/* Internal method, called by destructor to clean up assets. */
		void cleanup();

		void out(const LogLevel level, const std::string& message);
	};
}