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

#include <Meteorite/Network/DataStream.h>

#include <Meteorite/Network/ByteOrder.h>

#define reinterpret_bytes(type) *(reinterpret_cast<type*>(&((*buffer)[pos])))

namespace Meteorite
{
	namespace Network
	{
		DataStream::DataStream() : buffer(nullptr), pos(0), valid(false)
		{
		}

		void DataStream::reset()
		{
			buffer = nullptr;
			pos = 0;
			valid = false;
		}

		std::vector<uint8_t>* DataStream::getBuffer()
		{
			return buffer;
		}

		size_t DataStream::getPos()
		{
			return pos;
		}

		bool DataStream::isValid()
		{
			return valid;
		}

		void DataStream::setBuffer(std::vector<uint8_t>* buffer)
		{
			this->buffer = buffer;
		}

		void DataStream::setPos(size_t pos)
		{
			this->pos = pos;
		}

		void DataStream::setValid(bool valid)
		{
			this->valid = valid;
		}

		bool DataStream::hasBytes(size_t n)
		{
			return valid && (buffer->size() - pos) >= n;
		}

		size_t DataStream::countRemainingBytes()
		{
			return buffer->size() - pos;
		}
		
		void DataStream::skip(size_t n)
		{
			if (hasBytes(n))
			{
				pos += n;
				valid = true;
			}
			else
				valid = false;
		}

		void DataStream::append(const uint8_t* src, size_t n)
		{
			buffer->insert(buffer->end(), src, src + n);
		}

		void DataStream::read(uint8_t& dst)
		{
			if (hasBytes(1))
			{
				dst = (*buffer)[pos++];
				valid = true;
			}
			else
				valid = false;
		}

		void DataStream::read(int16_t& dst)
		{
			if (hasBytes(2))
			{
				dst = letoh16(reinterpret_bytes(int16_t));
				pos += 2;
				valid = true;
			}
			else
				valid = false;
		}

		void DataStream::read(int32_t& dst)
		{
			if (hasBytes(4))
			{
				dst = letoh32(reinterpret_bytes(int32_t));
				pos += 4;
				valid = true;
			}
			else
				valid = false;
		}

		void DataStream::read(float& dst)
		{
			if (hasBytes(4))
			{
				uint32_t res;
				uint8_t* p = reinterpret_cast<uint8_t*>(&res);

				for (size_t i = 0; i < sizeof(res); ++i)
					*p++ = (*buffer)[pos++];

				uint32_t ival = letoh32(res);
				memcpy(&dst, &ival, 4);
				valid = true;
			}
			else
				valid = false;
		}

		void DataStream::read(Meteorite::Network::Color& dst)
		{
			if (hasBytes(3))
			{
				read(dst.r);
				read(dst.g);
				read(dst.b);
			}
			else
				valid = false;
		}

		void DataStream::read(std::string& dst)
		{
			if (hasBytes(1))
			{
				uint8_t varintBuffer[10];

				if (hasBytes(10))
					memcpy(varintBuffer, &reinterpret_bytes(uint8_t), 10);
				else
					memcpy(varintBuffer, &reinterpret_bytes(uint8_t), countRemainingBytes());

				int i = 0;
				uint64_t stringLength = 0;
				int shiftAmount = 0;

				do
				{
					stringLength |= (uint64_t)(varintBuffer[i] & 0x7F) << shiftAmount;
					shiftAmount += 7;
				} while ((varintBuffer[i++] & 0x80) != 0);

				pos += i;

				if (!hasBytes((size_t) stringLength))
					valid = false;
				else
				{
					dst = std::string(buffer->begin() + pos, buffer->begin() + pos + (size_t) stringLength);
					pos += (size_t) stringLength;
				}
			}
			else
				valid = false;
		}

		void DataStream::write(const uint8_t& val)
		{
			buffer->push_back(val);
		}

		void DataStream::write(const int16_t& val)
		{
			uint16_t nval = htole16(val);
			append(reinterpret_cast<const uint8_t*>(&nval), sizeof(nval));
		}

		void DataStream::write(const int32_t& val)
		{
			uint32_t nval = htole32(val);
			append(reinterpret_cast<const uint8_t*>(&nval), sizeof(nval));
		}

		void DataStream::write(const float& val)
		{
			uint32_t nval;
			memcpy(&nval, &val, 4);
			nval = htole32(nval);
			append(reinterpret_cast<const uint8_t*>(&nval), sizeof(nval));
		}

		void DataStream::write(const Meteorite::Network::Color& val)
		{
			write(val.r);
			write(val.g);
			write(val.b);
		}

		void DataStream::remove()
		{
			buffer->erase(buffer->begin(), buffer->begin() + pos);
			pos = 0;
		}
	}
}