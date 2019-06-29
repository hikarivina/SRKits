// SRKits - An unofficial C/C++ API Wrapper for Silkroad Online.
// Copyright (C) 2019  Abdelhalem Samy (a.k.a. Klito)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License and/or GNU Affero General Public License 
// as published by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License and/or GNU Affero General Public License for more details.

// You should have received a copy of the GNU General Public License and/or GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <iostream>
#include <srkits/api/blowfish.h>

// Was taken from: https://github.com/Klito/Silkroad-Online-Second-Password-System
uint8_t key[8] = { 0x0F, 0x07, 0x3D, 0x20, 0x56, 0x62, 0xC9, 0xEB };
srkits::api::blowfish bf(key, sizeof(key));
std::string pin = "5689431"; // the pin length must be between 6 and 8 (SRO Rule, not me!)

void Example1()
{
	std::vector<uint8_t> encode_verify(bf.get_output_length(pin.length())); // set the output length for the encoded vector
	bf.encode(pin.data(), 0, pin.length(), encode_verify.data());           // @encode_verify now have the encoded pin

	std::vector<uint8_t> decode_verify(encode_verify.size());                       // set the output length for the decoded vector as same as the encoded vector
	bf.decode(encode_verify.data(), 0, encode_verify.size(), decode_verify.data()); // @decode_verify now have the decoded pin
}

void Example2()
{
	std::vector<uint8_t> verify(bf.get_output_length(pin.length())); // set the output length for the vector 
	                                                                 // the length will be good for both encoded and decoded buffers!

	bf.encode(pin.data(), 0, pin.length(), verify.data());     // @verify now have the encoded pin
	bf.decode(verify.data(), 0, verify.size(), verify.data()); // @verify now have the decoded pin
}

void Example3()
{
	std::vector<uint8_t> verify = bf.encode(pin.data(), 0, pin.length()); // @verify now have the encoded pin
	verify = bf.decode(verify.data(), 0, verify.size());                  // @verify now have the decoded pin
}

void Example4()
{
	uint8_t verify[8]; // fixed size, we are sure the encoded pin won't be length won't be > 8
	bf.encode(pin.data(), 0, pin.length(), &verify); // @verify now have the encoded pin
	bf.decode(&verify, 0, sizeof(verify),  &verify); // @verify now have the decoded pin
}
