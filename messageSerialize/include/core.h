#pragma once
#include <string>
#include <iostream>
#include <bitset>
#include <fstream>
#include <cstdint>
#include <vector>




namespace Core {
	namespace Util {
		inline void save(const char* file, std::vector<uint8_t>& buffer) {
			std::ofstream out;
			out.open(file);

			for (size_t i = 0; i < buffer.size(); i++) {
				out << buffer[i];
			}
			out.close();
		}
	} // namespace Util

	template<typename T>
	void encode(std::vector<uint8_t>& buffer, T value) {
		// bytes is a pointer to the beggining of memory, where value is stored
		// BUT interpareted as an array of bytes
		const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value); 
		buffer.insert(buffer.end(), bytes, bytes + sizeof(T));
	}

	template<>
	inline void encode<std::string>(std::vector<uint8_t>& buffer, std::string value) {
		for (size_t i = 0; i < value.size(); i++) {
			encode<uint8_t>(buffer, value[i]);
		}
	}

	template<typename T>
	inline T decode(const std::vector<uint8_t>& buffer, int16_t* iterator) {
		T value;
		std::memcpy(&value, buffer.data() + *iterator, sizeof(T));
		*iterator += sizeof(T);
		return value;
	}
	template<>
	inline std::string decode<std::string>(const std::vector<uint8_t>& buffer, int16_t* iterator) {
		
		uint16_t lenght = decode<uint16_t>(buffer, iterator);
		std::string value(buffer.begin() + *iterator, buffer.begin() + *iterator + lenght);
		*iterator += lenght;
		return value;
	}



} // namespace Core