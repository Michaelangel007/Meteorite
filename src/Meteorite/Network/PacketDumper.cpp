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

#include <Meteorite/Network/PacketDumper.h>

#include <iomanip>
#include <boost/filesystem.hpp>

namespace Meteorite
{
	namespace Network
	{
		PacketDumper::PacketDumper()
		{
		}

		PacketDumper::~PacketDumper()
		{
		}

		void PacketDumper::dump(uint8_t id, std::vector<uint8_t>* buffer, size_t startPos, size_t length)
		{
			std::vector<uint8_t> data;
			data.insert(data.begin(), buffer->begin() + startPos, buffer->begin() + startPos + length);
			
			std::string dir;
			std::stringstream ss;
			ss << "./dumped_packets/0x";
			ss << std::setfill('0') << std::setw(2) << std::hex << (int) id << "/";
			dir = ss.str();

			int index = 0;
			std::string filename;
			boost::filesystem::path path(dir);
			boost::filesystem::create_directories(path);

			while (true)
			{
				if (index == 99999)
					return;

				ss.str("");
				ss.clear();
				ss << std::setfill('0') << std::setw(5) << index << ".txt";
				filename = ss.str();
				path = boost::filesystem::path(dir + filename);

				if (!boost::filesystem::exists(path))
					break;

				++index;
			}

			std::ofstream file(path.generic_string());
			file << "Payload length: " << length << std::endl;
			file << "---------------------------" << std::endl;

			for (size_t i = 0; i < data.size(); ++i)
				file << data[i];

			file << std::endl;
			file.close();
		}
	}
}