#ifndef RC4_H
#define RC4_H

#include <vector>
#include <cstdint>

class RC4 {
public:
    void keyGenerate(const std::vector<uint8_t>& key);
    void encryptDecrypt(std::vector<uint8_t>& data);

private:
    std::vector<uint8_t> S;
    uint8_t i, j;
};

#endif // RC4_H