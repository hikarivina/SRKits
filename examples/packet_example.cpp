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

#include <iostream>

#include <srkits/api/packet.h>

int main()
{
  // You have to see `binarystream_example.cpp` because packet is just a binarystream
  uint8_t val = 0xA1;
  srkits::api::packet p(0xA101, 1, 0);
  
  for (size_t i = 0; i < 100; i++)
  {
    // Fill the packet with any data right now
    p << val;
  }

  std::cout << p.header_to_string() << std::endl;
  std::cout << p.dump_to_string() << std::endl;
  std::cin.get();
  return 0;
}