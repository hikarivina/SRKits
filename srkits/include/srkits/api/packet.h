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

#ifndef SRKITS_API_PACKET_H_
#define SRKITS_API_PACKET_H_

#include <srkits/api/binarystream.h>

namespace srkits
{
  namespace api 
  {

    class packet : public binarystream
    {
      public:
        uint16_t opcode;
        uint8_t encrypted;
        uint8_t massive;

        packet(uint16_t opcode, uint8_t encrypted = false, uint8_t massive = false);
        packet(uint16_t opcode, binarystream& stream, uint8_t encrypted = false, uint8_t massive = false);
        packet(uint16_t opcode, std::vector<uint8_t>& data, uint8_t encrypted = false, uint8_t massive = false);

        std::string header_to_string() const;
    }; // class packet : public binarystream

  } // namespace api
} // namespace srkits

#endif // #ifdef SRKITS_API_PACKET_H_