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

#ifndef SRKITS_API_BINARYSTREAM_H_
#define SRKITS_API_BINARYSTREAM_H_

#include <stdint.h>

#include <vector>
#include <string>
#include <iterator>
#include <cstring>

namespace srkits
{
	namespace api
	{

		enum SEEK_DIR
		{
			SEEK_BEGIN_STREAM,
			SEEK_FORWARD,
			SEEK_BACKWARD,
			SEEK_END_STREAM
		}; // enum SEEK_DIR

		class binarystream
		{
			protected:
				std::vector<uint8_t>& m_stream;
				std::vector<uint8_t> m_stream_internal;
				int32_t m_read_index;
				bool m_read_error;
				bool m_write_error;
				
			public:
				binarystream();
				binarystream(std::vector<uint8_t> & stream_data);
				binarystream(const void * const buffer, int32_t size);
				binarystream(const binarystream & rhs);
				~binarystream();
				binarystream & operator = (const binarystream & rhs);

			public:
				template <typename type>
				type read(bool peek = false)
				{
					type val = 0;
					read<type>(&val, 1, peek);
					return val;
				}

				template <typename type>
				void read(type * out, uint32_t count, bool peek = false)
				{
					if(count <= 0)
					{
						return;
					}
					if(m_read_error || m_read_index + (count * sizeof(type))> static_cast<int32_t>(m_stream.size()))
					{
						m_read_error = true;
					}
					else
					{
						memcpy(out, &m_stream[m_read_index], (count * sizeof(type)));
						if(!peek)
						{
							m_read_index += (count * sizeof(type));
						}
					}
				}

				template <typename type>
				void write(const std::vector<type> & val)
				{
					write<type>(val.empty() ? 0 : &val[0], static_cast<int32_t>(val.size()));
				}

				template <typename type>
				void write(type val)
				{
					write<type>(&val, 1);
				}

				template <typename type>
				void write(const type * const input, uint32_t count)
				{
					if(count)
					{
						std::copy((uint8_t *)input, (count * sizeof(type)) + (uint8_t *)input, std::back_inserter(m_stream));
					}
				}

				template <typename type>
				void insert(int32_t index, const std::vector<type> & val)
				{
					insert<type>(index, val.empty() ? 0 : &val[0], static_cast<int32_t>(val.size()));
				}

				template <typename type>
				void insert(int32_t index, type val)
				{
					insert<type>(index, &val, 1);
				}

				template <typename type>
				void insert(int32_t index, const type * const input, uint32_t count)
				{
					if(index >= static_cast<int32_t>(m_stream.size()))
					{
						m_stream.resize(static_cast<int32_t>(m_stream.size()) + (sizeof(type) * count));
					}
					else
					{
						m_stream.resize(m_stream.size() + (sizeof(type) * count));
						memmove(&m_stream[index + sizeof(type) * count], &m_stream[index], static_cast<int32_t>(m_stream.size()) - index - sizeof(type) * count);
					}
					memcpy(&m_stream[index], input, (count * sizeof(type)));
				}

				template <typename type>
				void overwrite(int32_t index, const std::vector<type> & val)
				{
					overwrite<type>(index, val.empty() ? 0 : &val[0], static_cast<int32_t>(val.size()));
				}

				template <typename type>
				void overwrite(int32_t index, type val)
				{
					overwrite<type>(index, &val, 1);
				}

				template <typename type>
				void overwrite(int32_t index, const type * const input, uint32_t count)
				{
					if(static_cast<int32_t>(m_stream.size()) <index + (count * sizeof(type)))
					{
						m_stream.resize(index + (count * sizeof(type)));
					}
					memcpy(&m_stream[index], input, count * sizeof(type));
				}

				template <typename type>
				void fill(int32_t index, type value, uint32_t count)
				{
					if(static_cast<int32_t>(m_stream.size()) <index + (sizeof(type) * count))
					{
						m_stream.resize(index + (sizeof(type) * count));
					}
					type * stream = reinterpret_cast<type *>(&m_stream[index]);
					for(uint32_t x = 0; x < count; ++x)
					{
						stream[x] = value;
					}
				}

			public:
				bool was_write_error() const;
				bool was_read_error() const;

				void clear_read_error();
				void clear_write_error();

				const std::vector<uint8_t> & get_stream_vector() const;
				const uint8_t * get_data() const;
				int32_t get_size() const;
				int32_t get_write_index() const;

				const uint8_t * get_read_stream_ptr();
				int32_t get_remaining_read() const;
				int32_t get_read_index() const;

				bool seek_read(int32_t index, SEEK_DIR dir);

				void clear_stream();
				int32_t delete_stream(int32_t index, uint32_t count);
				binarystream extract_stream(int32_t index, uint32_t count) const;

				std::string read_ascii(uint32_t count);
				std::wstring read_ascii_to_unicode(uint32_t count);
				std::wstring read_unicode(uint32_t count);
				std::string read_unicode_to_ascii(uint32_t count);

				void write_ascii(const std::string & mbs_text);
				void write_ascii(const char * mbs_text, uint32_t count);
				void write_ascii_to_unicode(const std::string & mbs_text);
				void write_ascii_to_unicode(const char * mbs_text, uint32_t count);
				void write_unicode(const std::wstring & wcs_text);
				void write_unicode(const wchar_t * wcs_text, uint32_t count);
				void write_unicode_to_ascii(const std::wstring & wcs_text);
				void write_unicode_to_ascii(const wchar_t * wcs_text, uint32_t count);

				std::string dump_to_string() const;
			
			public:
				template <typename type>
				binarystream& operator<< (const type& data)
				{
					this->write<type>(data);
					return *this;
				}

				binarystream& operator<< (const char* data);
				binarystream& operator<< (const wchar_t* data);
				binarystream& operator<< (const std::string& data);
				binarystream& operator<< (const std::wstring& data);

				template <typename type>
				binarystream& operator>> (type& data)
				{
					data = this->read<type>();
					return *this;
				}

				// TODO: Add operator>> support for c_str and wc_stc
				// binarystream& operator>> (char* data);
				// binarystream& operator>> (wchar_t* data);
				binarystream& operator>> (std::string& data);
				binarystream& operator>> (std::wstring& data);

				uint8_t operator[] (const uint32_t index);
		}; // class binarystream

	} // namespace api
} // namespace srkits

#endif // #ifdef SRKITS_API_BINARYSTREAM_H_
