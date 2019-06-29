// SRKits - An unofficial C/C++ API Wrapper for Silkroad Online.
// Copyright (C) 2019  Abdelhalem Samy (a.k.a. Klito)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License and/or GNU Affero General Public License 
// as published by the Free Software Foundation, either version 3 of the License, or any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License and/or GNU Affero General Public License for more details.

// You should have received a copy of the GNU General Public License and/or GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <srkits/api/packet.h>

#include <iostream>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <string>

namespace srkits
{
  namespace api 
  {

    packet::packet(uint16_t opcode, uint8_t encrypted, uint8_t massive)
    : binarystream(), opcode(opcode), encrypted(encrypted), massive(massive)
    {
      if (this->encrypted && this->massive)
      {
        throw std::exception("[srkits::api::packet] Packet cannot be encrypted and massive together.");
      }
    }

    packet::packet(uint16_t opcode, binarystream& stream, uint8_t encrypted, uint8_t massive)
    : binarystream(stream), opcode(opcode), encrypted(encrypted), massive(massive)
    {
      if (this->encrypted && this->massive)
      {
        throw std::exception("[srkits::api::packet] Packet cannot be encrypted and massive together.");
      }
    }

    packet::packet(uint16_t opcode, std::vector<uint8_t>& data, uint8_t encrypted, uint8_t massive)
    : binarystream(data), opcode(opcode), encrypted(encrypted), massive(massive)
    {
      if (this->encrypted && this->massive)
      {
        throw std::exception("[srkits::api::packet] Packet cannot be encrypted and massive together.");
      }
    }
    
    std::string packet::header_to_string() const
    {
      std::stringstream ss;
      int32_t size = static_cast< int32_t >(this->m_stream.size());
      
      ss << "[" << std::hex << std::setfill('0') << std::setw(4) << std::uppercase << opcode << "]";
      ss << "[" << std::dec << size << " bytes]";

      if (this->encrypted)
      {
          ss << "[Encrypted]"; 
      }
      if (this->massive)
      {
          ss << "[Massive]";
      }

      return ss.str();
    }

  } // namespace api
} // namespace srkits