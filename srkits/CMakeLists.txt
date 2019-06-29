# SRKits - An unofficial C/C++ API Wrapper for Silkroad Online.
# Copyright (C) 2019  Abdelhalem Samy (a.k.a. Klito)

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License and/or GNU Affero General Public License 
# as published by the Free Software Foundation, either version 3 of the License, or any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License and/or GNU Affero General Public License for more details.

# You should have received a copy of the GNU General Public License and/or GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

add_library(srkits STATIC src/api/blowfish.cpp src/api/binarystream.cpp src/api/packet.cpp)