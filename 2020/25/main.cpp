#include "../../common.h"

ull transform(const ull subject, const ull loops) {
    static Map<ull, Map<ull, ull>> cacheMaps;
    static Map<ull, ull> highestCaches;

    auto &cacheMap = cacheMaps[subject];

    auto it = cacheMap.find(loops);
    if (it != cacheMap.end()) {
        return it->second;
    }

    auto &highestCache = highestCaches[subject];

    if (loops < highestCache) {
        highestCache = 0;
    }

    ull value{1};

    if (highestCache != 0) {
        value = cacheMap[highestCache];
    }

    for (ull i{0}; i < loops - highestCache; ++i) {
        value *= subject;
        value = value % 20201227ull;
    }

    cacheMap[loops] = value;

    if (loops > highestCache) {
        highestCache = loops;
    }

    return value;
}

struct Device {
    ull publicKey;
    ull loop;
    ull encryptionKey;

    void computeLoop() {
        ull i{1};

        while (true) {

            if (publicKey == transform(7, i)) {
                loop = i;
                return;
            }

            ++i;
        }
    }

    void computeEncryptionKey(const ull otherPublicKey) {
        encryptionKey = transform(otherPublicKey, loop);
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Device card{std::stoull(lines[0])};
    Device door{std::stoull(lines[1])};

    door.computeLoop();

    door.computeEncryptionKey(card.publicKey);
    log << "Part1 : " << door.encryptionKey << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
