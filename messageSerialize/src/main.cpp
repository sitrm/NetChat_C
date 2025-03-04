#include "../include/message.h"
#include "../include/core.h"


using namespace Core;

int main() {

    Core::Message msg = Core::Message("user", "hello");
    std::string file = "test";
    std::vector<uint8_t> buffer = msg.serialize();

    Util::save(file.c_str(), buffer);

    //int16_t it = 0;
    //std::string val = Core::decode<std::string>(buffer, &it);
    //uint16_t l = Core::decode<uint16_t>(buffer, &it);
    //
    Message m = Core::Message::deserialize(buffer);
    return 0;
}