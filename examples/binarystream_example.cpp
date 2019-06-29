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

#include <srkits/api/binarystream.h>

int main() 
{
  uint8_t flag = 1;
  int id = 35681658;
  std::string name = "Klito";

  srkits::api::binarystream bs;

  // -- WRITING -------------------------------
  bs.write<uint8_t>(flag);
  bs.write<int>(id);
  bs.write<uint16_t>((uint16_t)name.length()); // In SRO streams the string length is 2 bytes not 4 as std::string.length() return
  bs.write_ascii(name);
  // -----------------------------------------
  bs.clear_stream(); // erase the stream and start a new one!
  // -----------------------------------------
  bs << flag << id << (uint16_t)name.length() << name; // In SRO streams the string length is 2 bytes not 4 as std::string.length() return
  // -----------------------------------------

  // -- READING ------------------------------
  uint16_t name_length = 0;

  flag = bs.read<uint8_t>();
  id = bs.read<int>();
  name_length = bs.read<uint16_t>(); // We have already wrote the length into the stream, so we will it for reading otherwise it will be hardcoded!
  name = bs.read_ascii(name_length);
  // -----------------------------------------
  bs.seek_read(0, srkits::api::SEEK_BEGIN_STREAM); // Reset the reader index to the begin of the stream
  // -----------------------------------------
  bs >> flag >> id >> name_length;
  name.resize(name_length); // You have to resize the string before reading using operator>>
  bs >> name;
  // -----------------------------------------
  try 
  {
    uint8_t byte_by_index = bs[bs.get_size() + 1];
  }
  catch (std::exception& e)
  { // In this example an exception will be thrown no way, because of `bs.get_size() + 1`
    std::cout << e.what() << std::endl;
  }
  // -----------------------------------------

  // -- WARNING ------------------------------
  bs.clear_stream(); // erase the stream and start a new one!
  // -----------------------------------------
  bs << (uint16_t)name.length() << name;
  char* c_name = "";
  bs >> c_name;     // using operator>> with c_str is not supported right now
                    // and it will lead to unexpected behavior!
  // -----------------------------------------

  std::cin.get();
  return 0;
}