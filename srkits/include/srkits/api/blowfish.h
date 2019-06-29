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

#ifndef SRKITS_API_BLOWFISH_H_
#define SRKITS_API_BLOWFISH_H_

#include <stdint.h>

#include <cstddef>
#include <vector>

namespace srkits 
{
  namespace api 
  {

    class blowfish 
    {
      public:
        blowfish(const void* const key, uint32_t length);

      public:
        void initialize(const void* const key, uint32_t length);
        std::vector<uint8_t> encode(const void* const src, uint32_t offset, uint32_t length) const;
        bool encode(const void* const src, uint32_t offset, uint32_t length, void* dst) const;
        std::vector<uint8_t> decode(const void* const src, uint32_t offset, uint32_t length) const;
        bool decode(const void* const src, uint32_t offset, uint32_t length, void* dst) const;

        inline int32_t get_output_length(uint32_t length) const;
        inline size_t get_padding_length_enc(uint32_t length) const;
        size_t get_padding_length_dec(const void* const data, uint32_t length) const;

      private:
        void encipher(uint32_t* left, uint32_t* right) const;
        void decipher(uint32_t* left, uint32_t* right) const;
        uint32_t feistel(uint32_t value) const;

      private:
          uint32_t pary_[18];
          uint32_t sbox_[4][256];
    }; // class blowfish

  } // namespace api
} // namespace srkits

#endif // #ifndef SRKITS_API_BLOWFISH_H_
