#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <bitset>
#include <cstdint>
#include <vector>



namespace Core {
	class Message {
	private:
		std::string username;
		std::string message;
		
		uint16_t  usernameSize;
		uint16_t messageSize;

		uint32_t size;

		std::vector<uint8_t>* buffer;
	public:
		Message(const std::string& name, const std::string& message);
		~Message() = default;
	public:
		std::string getUsername()  const { return this->username;     }
		std::string getMessage()   const { return this->message;      }
		uint16_t getUsernameSize() const { return this->usernameSize; }
		uint16_t getMessageSize()  const { return this->messageSize;  }
	public:
		std::vector<uint8_t> serialize();
		static Message deserialize(const std::vector<uint8_t>& buffer);

	}; // class Message
} // namespace Core