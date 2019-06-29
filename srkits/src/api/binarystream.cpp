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

// We don't want any secure warnings 
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "srkits/api/binarystream.h"

#include <iostream>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <string>

namespace	srkits 
{
	namespace api 
	{

		binarystream::binarystream() : m_stream(m_stream_internal)
		{
			this->clear_stream();
		}

		binarystream::binarystream(std::vector<uint8_t> & stream) : m_stream(stream)
		{
			this->m_read_index = 0;
			this->m_read_error = false;
			this->m_write_error = 0;
		}

		binarystream::binarystream(const void * const buffer, int32_t size) : m_stream(m_stream_internal)
		{
			this->clear_stream();
			this->overwrite<uint8_t>(0, reinterpret_cast<const uint8_t *>(buffer), size);
		}

		binarystream::binarystream(const binarystream & rhs)
		: m_stream(m_stream_internal), m_read_index (rhs.m_read_index), m_read_error(rhs.m_read_error), m_write_error(rhs.m_write_error)
		{
			this->m_stream = rhs.m_stream;
		}

		binarystream::~binarystream()
		{
		}

		binarystream & binarystream::operator =(const binarystream & rhs)
		{
			if(&rhs != this)
			{
				this->m_stream = rhs.m_stream;
				this->m_read_index = rhs.m_read_index;
				this->m_read_error = rhs.m_read_error;
				this->m_write_error = rhs.m_read_error;
			}
			return *this;
		}

		void binarystream::clear_stream()
		{
			this->m_stream.clear();
			this->m_read_index = 0;
			this->m_read_error = false;
			this->m_write_error = false;
		}

		bool binarystream::was_write_error() const
		{
			return this->m_write_error;
		}

		bool binarystream::was_read_error() const
		{
			return this->m_read_error;
		}

		void binarystream::clear_read_error()
		{
			this->m_read_error = false;
		}

		void binarystream::clear_write_error()
		{
			this->m_write_error = false;
		}

		const std::vector<uint8_t> & binarystream::get_stream_vector() const
		{
			return this->m_stream;
		}

		const uint8_t * binarystream::get_data() const
		{
			return this->m_stream.data();
			// return this->m_stream.empty() ? 0 : &this->m_stream[0];
		}

		int32_t binarystream::get_size() const
		{
			return static_cast<int32_t>(this->m_stream.size());
		}

		int32_t binarystream::get_write_index() const
		{
			return static_cast<int32_t>(this->m_stream.size()) - 1;
		}

		const uint8_t * binarystream::get_read_stream_ptr()
		{
			if(this->m_read_index == 0 || this->m_read_index >= static_cast<int32_t>(this->m_stream.size()))
			{
				return NULL;
			}
			return &this->m_stream[this->m_read_index];
		}

		int32_t binarystream::get_remaining_read() const
		{
			if(this->m_read_index >= static_cast<int32_t>(this->m_stream.size()))
			{
				return 0;
			}
			return static_cast<int32_t>(this->m_stream.size()) - this->m_read_index;
		}

		int32_t binarystream::get_read_index() const
		{
			return this->m_read_index;
		}

		bool binarystream::seek_read(int32_t index, SEEK_DIR dir)
		{
			int32_t old_read_index = this->m_read_index;
			if(dir == SEEK_BEGIN_STREAM)
			{
				this->m_read_index = index;
			}
			else if(dir == SEEK_FORWARD)
			{
				this->m_read_index += index;
			}
			else if(dir == SEEK_BACKWARD)
			{
				this->m_read_index -= index;
			}
			else if(dir == SEEK_END_STREAM)
			{
				this->m_read_index = static_cast<int32_t>(this->m_stream.size()) - 1;
				this->m_read_index -= index;
			}
			if(this->m_read_index > static_cast<int32_t>(this->m_stream.size()))
			{
				this->m_read_index = old_read_index;
				return false;
			}
			return true;
		}

		int32_t binarystream::delete_stream(int32_t index, uint32_t count)
		{
			uint32_t delcount = 0;
			if(index <static_cast<int32_t>(this->m_stream.size()))
			{
				delcount = static_cast<int32_t>(this->m_stream.size()) - index;
				if(delcount> count)
				{
					delcount = count;
				}
			}
			if(delcount)
			{
				this->m_stream.erase(this->m_stream.begin() + index, this->m_stream.begin() + index + delcount);
				if(this->m_read_index> static_cast<int32_t>(this->m_stream.size()))
				{
					this->m_read_index = static_cast<int32_t>(this->m_stream.size());
				}
			}
			return delcount;
		}

		binarystream binarystream::extract_stream(int32_t index, uint32_t count) const
		{
			binarystream result;
			if(count == -1)
			{
				count = static_cast<int32_t>(this->m_stream.size()) - index;
			}
			if(count && index + count <= static_cast<int32_t>(this->m_stream.size()))
			{
				result.write(&this->m_stream[index], count);
			}
			return result;
		}

		std::string binarystream::read_ascii(uint32_t count)
		{
			if(count <= 0)
			{
				return "";
			}
			std::string str;
			str.resize(count);
			this->read<char>(&str[0], count);
			if(this->m_read_error)
			{
				str.clear();
			}
			return str;
		}

		std::wstring binarystream::read_ascii_to_unicode(uint32_t count)
		{
			if(count <= 0)
			{
				return L"";
			}
			std::string str;
			str.resize(count);
			this->read<char>(&str[0], count);
			if(this->m_read_error)
			{
				return L"";
			}
			int32_t converted = static_cast<int32_t>(mbstowcs(NULL, str.c_str(), count));
			if(converted == 0 || (converted - 1) != count)
			{
				this->m_read_error = true;
				return L"";
			}
			std::wstring wcsStr;
			wcsStr.resize(converted);
			mbstowcs(&wcsStr[0], str.c_str(), count);
			wcsStr.erase(wcsStr.end() - 1); // remove extra null terminator
			return wcsStr;
		}

		std::wstring binarystream::read_unicode(uint32_t count)
		{
			if(count <= 0)
			{
				return L"";
			}
			std::wstring str;
			str.resize(count);
			this->read<wchar_t>(&str[0], count);
			if(this->m_read_error)
			{
				str.clear();
			}
			return str;
		}

		void binarystream::write_ascii(const std::string & mbs_text)
		{
			this->write_ascii(mbs_text.c_str(), static_cast<int32_t>(mbs_text.size()));
		}

		void binarystream::write_ascii(const char * mbs_text, uint32_t count)
		{
			this->write<char>(mbs_text, count);
		}

		void binarystream::write_ascii_to_unicode(const std::string & mbs_text)
		{
			this->write_ascii_to_unicode(mbs_text.c_str(), static_cast<int32_t>(mbs_text.size()));
		}

		void binarystream::write_ascii_to_unicode(const char * mbs_text, uint32_t count)
		{
			if(count <= 0)
			{
				return;
			}
			int32_t converted = static_cast<int32_t>(mbstowcs(NULL, mbs_text, count));
			if(converted == 0 || (converted - 1) != count)
			{
				this->m_write_error = true;
				return;
			}
			std::wstring wcsStr;
			wcsStr.resize(converted);
			mbstowcs(&wcsStr[0], mbs_text, count);
			this->write_unicode(wcsStr.c_str(), converted - 1);
		}

		void binarystream::write_unicode(const std::wstring & wcs_text)
		{
			this->write_unicode(wcs_text.c_str(), static_cast<int32_t>(wcs_text.size()));
		}
		
		void binarystream::write_unicode(const wchar_t * wcs_text, uint32_t count)
		{
			this->write<wchar_t>(wcs_text, count);
		}
		
		void binarystream::write_unicode_to_ascii(const std::wstring & wcs_text)
		{
			this->write_unicode_to_ascii(wcs_text.c_str(), static_cast<int32_t>(wcs_text.size() ));
		}

		void binarystream::write_unicode_to_ascii(const wchar_t * wcs_text, uint32_t count)
		{
			if(count <= 0)
			{
				return;
			}
			int32_t converted = static_cast<int32_t>(wcstombs(NULL, wcs_text, count));
			if(converted == 0 || (converted - 1) != count)
			{
				this->m_write_error = true;
				return;
			}
			std::string mbsStr;
			mbsStr.resize(converted);
			wcstombs(&mbsStr[0], wcs_text, count);
			this->write_ascii(mbsStr.c_str(), converted - 1);
		}

#define BYTES_PER_LINE 16
		std::string binarystream::dump_to_string() const
		{
			std::stringstream output;
			std::stringstream output_ascii;

			int32_t size = static_cast<int32_t>(this->m_stream.size());
			int32_t length = size;
			if (length % BYTES_PER_LINE != 0)
			{
				length += BYTES_PER_LINE - length % BYTES_PER_LINE;
			}

			for (int32_t i = 0; i <= length; ++i)
			{
				if (i % BYTES_PER_LINE == 0)
				{
					if (i > 0)
					{
						output << " " << output_ascii.str() << "\n";
						output_ascii.clear(1);
					}
					if (i != length)
					{
						output << std::dec << std::setfill('0') << std::setw(10) << i << "   ";
					}
				}
				if (i < size)
				{
					int ch = (i >= size ? 0 : this->m_stream[i]);
					output << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ch << " ";
					if (isprint(ch) && !isspace(ch))
					{
						output_ascii << (char)ch;
					}
					else
					{
						output_ascii << ".";
					}
				}
				else
				{
					output << (i == length ? " " : "00 ");
					output_ascii << ".";
				}
			}

			return output.str();	
		}

		binarystream& binarystream::operator<< (const char* data)
		{
			std::string str(data);
			this->write_ascii(str);
			return *this;
		}

		binarystream& binarystream::operator<< (const wchar_t* data)
		{
			std::wstring wstr(data);
			this->write_unicode(wstr);
			return *this;
		}

		binarystream& binarystream::operator<< (const std::string& data)
		{
			this->write_ascii(data);
			return *this;
		}

		binarystream& binarystream::operator<< (const std::wstring& data)
		{
			this->write_unicode(data);
			return *this;
		}

		binarystream& binarystream::operator>> (std::string& data)
		{
			data = this->read_ascii(data.length());
			return *this;
		}

		binarystream& binarystream::operator>> (std::wstring& data)
		{
			data = this->read_unicode(data.length());
			return *this;
		}
		
		uint8_t binarystream::operator[] (const uint32_t index)
		{
			if (index >= this->m_stream.size())
			{
				throw std::exception("[srkits::api::binarystream] Index out of range.");
			}
			return this->m_stream[index];
		}

	} // namespace api
} // namespace srkits