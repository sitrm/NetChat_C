#include "message.h"
#include "core.h"

using namespace Core;


Message::Message(const std::string& name, const std::string& message)
	:
	username(name),
	message(message),
	usernameSize(static_cast<uint16_t>(name.size())),
	messageSize(static_cast<uint16_t>(message.size())),
	size(sizeof(usernameSize) + usernameSize + sizeof(messageSize) + messageSize) {}


std::vector<uint8_t> Message::serialize() {
	std::vector<uint8_t> buffer;
	
	Core::encode<uint16_t>(buffer, usernameSize);
	Core::encode<std::string>(buffer, username);
	Core::encode<uint16_t>(buffer, messageSize);
	Core::encode<std::string>(buffer, message);

	return buffer;
}

Message Core::Message::deserialize(const std::vector<uint8_t>& buffer)
{
	int16_t it = 0;

	std::string username = Core::decode<std::string>(buffer, &it);
	std::string message = Core::decode<std::string>(buffer, &it);


	return Message(username, message);
}


