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
		struct Color
		{
			uint8_t r, g, b;
		};

		class DataStream
		{
		public:
			DataStream();

			/*
			 * Resets the DataStream.
			 */
			void reset();

			/*
			 * Returns a pointer to the buffer.
			 */
			std::vector<uint8_t>* getBuffer();

			/*
			 * Returns the current position of the DataStream.
			 */
			size_t getPos();

			/*
			 * Returns whether or not the DataStream is valid.
			 */
			bool isValid();

			/*
			 * Sets the current buffer to be used by the DataStream.
			 */
			void setBuffer(std::vector<uint8_t>* buffer);

			/*
			 * Sets the current position of the DataStream.
			 */
			void setPos(size_t pos);

			/*
			 * Sets whether or not the DataStream is valid.
			 */
			void setValid(bool valid);

			/*
			 * Returns true if the buffer has at least the specified number
			 * of bytes remaining in the buffer.
			 */
			bool hasBytes(size_t n);

			/*
			 * Returns the amount of bytes remaining in the buffer.
			 */
			size_t countRemainingBytes();

			/*
			 * Skips n bytes.
			 */
			void skip(size_t n);

			void append(const uint8_t* src, size_t n);

			void read(uint8_t& dst);
			void read(int16_t& dst);
			void read(int32_t& dst);
			void read(float& dst);
			void read(Meteorite::Network::Color& dst);
			void read(std::string& dst);

			void write(const uint8_t& val);
			void write(const int16_t& val);
			void write(const int32_t& val);
			void write(const float& val);
			void write(const Meteorite::Network::Color& val);

			/*
			 * Removes everything read thusfar from the buffer.
			 */
			void remove();

		private:
			std::vector<uint8_t>* buffer;
			size_t pos;
			bool valid;
		};
	}
}