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

#include <Meteorite/Logger.h>

#include <boost/date_time.hpp>

namespace Meteorite
{
	Logger::Logger(const LoggerSettings& settings)
	{
		this->settings = settings;
	}

	Logger::~Logger()
	{
		if (initialised)
			cleanup();
	}

	bool Logger::initialise()
	{
		if (initialised)
			return false;

		if (settings.outputToFile)
		{
			logFile.open(settings.filePath);

			if (!logFile.is_open())
				return false;
		}

		initialised = true;
		return true;
	}

	void Logger::cleanup()
	{
		if (initialised)
		{
			if (settings.outputToFile && logFile.is_open())
			{
				logFile.close();
			}
		}

		initialised = false;
	}

	void Logger::setLevel(const LogLevel level)
	{
		currentLevel = level;
	}

	void Logger::fatal(const std::string& message)
	{
		out(LOG_LEVEL_FATAL, message);
	}

	void Logger::error(const std::string& message)
	{
		out(LOG_LEVEL_ERROR, message);
	}

	void Logger::warning(const std::string& message)
	{
		out(LOG_LEVEL_WARNING, message);
	}

	void Logger::info(const std::string& message)
	{
		out(LOG_LEVEL_INFORMATION, message);
	}

	void Logger::debug(const std::string& message)
	{
		out(LOG_LEVEL_DEBUG, message);
	}

	void Logger::out(const LogLevel level, const std::string& message)
	{
		if (!initialised)
			return;

		if (currentLevel < level)
			return;

		std::stringstream ss;

		if (settings.enableTimestamps)
		{
			// idea: use ctime instead? probably faster and can use sprintf which avoids
			// using std::setw / std::setfill (which I imagine are much slower)
			// note to self: benchmark boost timestamp against ctime

			// Generate and output timestamp
			boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

			// YYYY-MM-DD
			ss << "[" <<
				std::setw(4) << std::setfill('0') << now.date().year() << "-" <<
				std::setw(2) << std::setfill('0') << now.date().month().as_number() << "-" <<
				std::setw(2) << std::setfill('0') << now.date().day().as_number() << " ";

			// HH:mm:SS
			ss <<
				std::setw(2) << std::setfill('0') << now.time_of_day().hours() << ":" <<
				std::setw(2) << std::setfill('0') << now.time_of_day().minutes() << ":" <<
				std::setw(2) << std::setfill('0') << now.time_of_day().seconds() << "] ";
		}

		switch (level)
		{
		case LOG_LEVEL_FATAL:
			ss << "[   FATAL]";
			break;
		case LOG_LEVEL_ERROR:
			ss << "[   ERROR]";
			break;
		case LOG_LEVEL_WARNING:
			ss << "[ WARNING]";
			break;
		case LOG_LEVEL_INFORMATION:
			ss << "[    INFO]";
			break;
		case LOG_LEVEL_DEBUG:
			ss << "[   DEBUG]";
			break;
		}

		ss << " " << message;

		if (settings.outputToStdOut)
			std::cout << ss.str() << std::endl;
		if (settings.outputToFile && logFile)
			logFile << ss.str() << std::endl;
	}

	void Logger::lineBreak(const LogLevel level)
	{
		if (currentLevel < level)
			return;

		if (settings.outputToStdOut)
			std::cout << std::endl;
		if (settings.outputToFile && logFile)
			logFile << std::endl;
	}
}