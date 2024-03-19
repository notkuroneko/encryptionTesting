#include "RC4.h"
#include <algorithm>

void RC4::keyGenerate(const std::vector<uint8_t>& key) {
    S.resize(256);
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key.size()]) % 256;
        std::swap(S[i], S[j]);
    }

    i = j = 0;
}

void RC4::encryptDecrypt(std::vector<uint8_t>& data) {
    for (size_t k = 0; k < data.size(); k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        std::swap(S[i], S[j]);
        data[k] ^= S[(S[i] + S[j]) % 256];
    }
}