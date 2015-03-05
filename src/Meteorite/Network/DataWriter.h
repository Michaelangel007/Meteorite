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

#include <Meteorite/Common.h>

namespace Meteorite
{
	namespace Network
	{
		class DataWriter
		{
		public:
			DataWriter();

			/*
			 * Appends n bytes from the src array to the buffer.
			 */
			void append(const uint8_t* src, const size_t n);

			void write(const uint8_t& val);
			void write(const int16_t& val);
			void write(const int32_t& val);
			void write(const float& val);
			void write(const std::string& val);

			/*
			 * Resets the DataWriter.
			 */
			void reset();

			/*
			 * Sets the current buffer to be written to by the DataWriter.
			 */
			void setBuffer(std::vector<uint8_t>* buffer);

			/*
			 * Returns a pointer to the buffer being written to by the DataWriter.
			 */
			std::vector<uint8_t>* getBuffer();

		private:
			std::vector<uint8_t>* buffer = nullptr;
		};
	}
}