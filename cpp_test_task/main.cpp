#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class Packet_Status {
	INCORRECT_HEADER,
	INCOMPLETE_PACKET,
	COMPLETE_PACKET
};

class Packet {
private:
	const uint8_t HEADER_1 = 0xDA;
	const uint8_t HEADER_2 = 0xDB;
	const int MAX_LENGTH = 65535;

	int payload_length = 0;
	std::vector<uint8_t> payload;
public:
	Packet_Status packet_validation(std::vector<uint8_t>& buffer) {
		if (buffer[0] != HEADER_1) {
			return Packet_Status::INCORRECT_HEADER;
		}

		if (buffer.size() > 1 && buffer[1] != HEADER_2) {
			return Packet_Status::INCORRECT_HEADER;
		}

		if (buffer.size() < 4) {
			return Packet_Status::INCOMPLETE_PACKET;
		}

		payload_length = (buffer[2] << 8) | buffer[3];

		if (payload_length + 4 > buffer.size()) {
			return Packet_Status::INCOMPLETE_PACKET;
		}

		return Packet_Status::COMPLETE_PACKET;
	}

	void parse(std::vector<uint8_t>& data) {
		uint8_t payload_length = (data[2] << 8) | data[3];

		payload.resize(payload_length);

		for (uint8_t i = 0; i < payload_length; i++)
		{
			payload[i] = data[i + 4];
		}
	}

	void print_payload() {
		for (uint8_t byte : payload)
		{
			std::cout << byte;
		}
		std::cout << std::endl;
	}
};

int main() {
	std::vector<uint8_t> buffer;
	Packet packet;

	while (true) {

		unsigned short symbol;
		std::cin >> std::hex >> symbol;


		buffer.push_back(symbol);

		Packet_Status packet_status = packet.packet_validation(buffer);

		if (packet_status == Packet_Status::INCOMPLETE_PACKET) {
			continue;
		}

		if (packet_status == Packet_Status::COMPLETE_PACKET) {
			packet.parse(buffer);
			packet.print_payload();
			buffer.clear();
			continue;
		}

		if (packet_status == Packet_Status::INCORRECT_HEADER) { // В случае если пришел некорректный заголовок, ждем когда придет корректный
			std::cerr << "Incorrect header" << std::endl;
			buffer.clear();
		}
	}

	return 0;
}