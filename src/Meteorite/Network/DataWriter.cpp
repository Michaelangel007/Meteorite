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

#include <Meteorite/Network/DataWriter.h>

#include <Meteorite/Network/ByteOrder.h>

namespace Meteorite
{
	namespace Network
	{
		DataWriter::DataWriter()
		{
		}

		void DataWriter::append(const uint8_t* src, const size_t n)
		{
			if (src == nullptr)
				return;

			buffer->insert(buffer->end(), src, src + n);
		}

		void DataWriter::write(const uint8_t& val)
		{
			append(reinterpret_cast<const uint8_t*>(&val), sizeof(val));
		}

		void DataWriter::write(const int16_t& val)
		{
			int16_t nval = htole16(val);
			append(reinterpret_cast<const uint8_t*>(&nval), sizeof(nval));
		}

		void DataWriter::write(const int32_t& val)
		{
			int32_t nval = htole32(val);
			append(reinterpret_cast<const uint8_t*>(&nval), sizeof(nval));
		}

		void DataWriter::write(const float& val)
		{
			uint32_t nval;
			memcpy(&nval, &val, 4);
			nval = htole32(nval);
			append(reinterpret_cast<const uint8_t*>(&nval), sizeof(nval));
		}

		void DataWriter::write(const std::string& val)
		{
			uint8_t varint[10];
			uint64_t length = (uint64_t) val.length();
			length = htole64(length);

			int i = 0;
			do
			{
				uint8_t nextByte = length & 0x7F;
				length >>= 7;

				if (length)
					nextByte |= 0x80;

				varint[i++] = nextByte;
			} while (length);

			append(varint, i);
			append(reinterpret_cast<const uint8_t*>(val.c_str()), val.length());
		}

		void DataWriter::reset()
		{
			buffer = nullptr;
		}

		void DataWriter::setBuffer(std::vector<uint8_t>* buffer)
		{
			this->buffer = buffer;
		}

		std::vector<uint8_t>* DataWriter::getBuffer()
		{
			return buffer;
		}
	}
}