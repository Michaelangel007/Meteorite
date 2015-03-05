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
		class DataReader
		{
		public:
			DataReader();

			void read(uint8_t& dst);
			void read(int16_t& dst);
			void read(int32_t& dst);
			void read(float& dst);
			void read(std::string& dst);

			/*
			* Resets the DataReader.
			*/
			void reset();

			/*
			* Skips n bytes.
			*/
			void skip(size_t n);

			/*
			* Removes everything read thusfar from the buffer.
			*/
			void trim();

			/*
			* Returns true if the buffer has at least the specified number
			* of bytes remaining in the buffer.
			*/
			bool hasBytes(size_t n);

			/*
			* Sets the current buffer to be read from by the DataReader.
			*/
			void setBuffer(std::vector<uint8_t>* buffer);

			/*
			* Sets the current position of the DataReader.
			*/
			void setPos(size_t pos);

			/*
			 * Returns a pointer to the buffer being read from by the DataReader.
			 */
			std::vector<uint8_t>* getBuffer();

			/*
			 * Returns the current position of the DataReader.
			 */
			size_t getPos();

			/*
			 * Returns true if the DataReader is still valid. If it is false,
			 * some of the read() calls failed and the message(s) weren't read
			 * successfully.
			 */
			bool isValid();
		private:
			std::vector<uint8_t>* buffer = nullptr;
			size_t pos = 0;
			bool valid = false;
		};
	}
}