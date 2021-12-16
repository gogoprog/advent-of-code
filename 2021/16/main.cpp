#include "../../common.h"

Map<char, String> hexaToBits = {{'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
                                {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
                                {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
                                {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};

struct Packet {
    int version;
    int typeId;

    ull value;
};

Map<int, std::function<ull(const Vector<Packet> &)>> operations = {

    {0,
     [](auto &packets) {
         return std::accumulate(packets.begin(), packets.end(), 0ull,
                                [](const ull res, const Packet &packet) { return res + packet.value; });
     }},
    {1,
     [](auto &packets) {
         return std::accumulate(packets.begin(), packets.end(), 1ull,
                                [](const ull res, const Packet &packet) { return res * packet.value; });
     }},
    {2,
     [](auto &packets) {
         auto &packet = *std::min_element(packets.begin(), packets.end(),
                                          [](const Packet &a, const Packet &b) { return a.value < b.value; });

         return packet.value;
     }},
    {3,
     [](auto &packets) {
         auto &packet = *std::max_element(packets.begin(), packets.end(),
                                          [](const Packet &a, const Packet &b) { return a.value < b.value; });

         return packet.value;
     }},
    {5, [](auto &packets) { return packets[0].value > packets[1].value ? 1 : 0; }},
    {6, [](auto &packets) { return packets[0].value < packets[1].value ? 1 : 0; }},
    {7, [](auto &packets) { return packets[0].value == packets[1].value ? 1 : 0; }},

};

struct Reader {
    StringView input;
    int cursor{0};

    static ull readValue(const StringView binary) {
        ull value;
        std::from_chars(binary.data(), binary.data() + binary.size(), value, 2);
        return value;
    }

    ull readValue(const int len) {
        auto result = readValue({input.data() + cursor, input.data() + cursor + len});
        cursor += len;
        return result;
    }

    StringView readAsString(const int len) {
        StringView result = {input.data() + cursor, input.data() + cursor + len};
        cursor += len;
        return result;
    }

    Vector<Packet> parse(int &vsum, const int max_packets = -1) {

        Vector<Packet> result;

        while (cursor < input.size() && (max_packets == -1 || result.size() < max_packets)) {

            Packet packet;

            packet.version = readValue(3);
            vsum += packet.version;
            packet.typeId = readValue(3);

            switch (packet.typeId) {
                case 4: {
                    String bits;

                    while (readValue(1) == 1) {
                        bits += readAsString(4);
                    }

                    bits += readAsString(4);

                    packet.value = readValue(bits);

                } break;

                default: {

                    int length_type_id = readValue(1);

                    Vector<Packet> subpackets;

                    switch (length_type_id) {
                        case 0: {

                            int total_length = readValue(15);

                            Reader reader{{input.data() + cursor, input.data() + cursor + total_length}};
                            subpackets = reader.parse(vsum);
                            cursor += total_length;
                        } break;
                        case 1: {

                            int subpackets_count = readValue(11);

                            Reader reader{{input.data() + cursor, input.data() + input.size()}};
                            subpackets = reader.parse(vsum, subpackets_count);

                            cursor += reader.cursor;

                        } break;
                    }


                    packet.value = operations[packet.typeId](subpackets);

                } break;
            }

            result.push_back(packet);
        }

        return result;
    }
};

void decode(const String line) {

    String binary;

    for (auto c : line) {
        binary += hexaToBits[c];
    }

    log << line << endl;

    auto vsum = 0;
    Reader reader(binary);
    auto packets = reader.parse(vsum, 1);

    log << "Part1: " << vsum << endl;
    log << "Part2: " << packets[0].value << endl;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    for (auto &line : lines) {
        decode(line);
    }
}

int main() {
    /* decode("D2FE28"); */
    /* decode("38006F45291200"); */
    /* decode("EE00D40C823060"); */
    /* decode("8A004A801A8002F478"); */
    /* decode("620080001611562C8802118E34"); */
    /* decode("C0015000016115A2E0802F182340"); */
    /* decode("A0016C880162017C3686B18A3D4780"); */
    decode("C200B40A82");
    decode("04005AC33890");
    decode("880086C3E88112");
    decode("CE00C43D881120");
    decode("D8005AC2A8F0");
    decode("F600BC2D8F");
    decode("9C005AC2F8F0");
    decode("9C0141080250320F1802104A08");
    process("input.txt");
    return 0;
}
