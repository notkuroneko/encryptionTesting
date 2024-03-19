#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono> // For timing
#include <thread>
#include "RC4.h"

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#else
#include <x86intrin.h>
#endif

using namespace std;

//Universal variable




double CPUspeed(){
    unsigned long long start, end;

    start = __rdtsc();

    auto start_time = std::chrono::steady_clock::now();

    // Wait for some time (e.g., 1 second)
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto end_time = std::chrono::steady_clock::now();

    end = __rdtsc();

    std::chrono::duration<double> time_diff = end_time - start_time;

    unsigned long long cycle_diff = end - start;

    double frequency = cycle_diff / time_diff.count();

    return frequency;

}

// void keyGenerate(vector<uint8_t> seed); // enter your KeyGenerate
// void encrypt(vector<uint8_t> data); // enter your KeyGenerate


void RC4encryptionRate(){
    vector<uint8_t> seed;
    seed.resize(18); // 128 bits key
    vector<uint8_t> data;
    data.resize(4096); // Resize the data to 4kB.
    for (uint8_t& byte : data) {
        byte = rand() % 256; // Randomize the data
    }
    for (uint8_t& byte : seed) {
        byte = rand() % 256; // Randomize the seed
    }
    RC4 rc4;
    rc4.keyGenerate(seed);
    size_t bitsEncrypted = 0;
    int blocksEncrypted = 0;
    auto start = chrono::high_resolution_clock::now();
    unsigned long long start_cycles = __rdtsc();
    do {
        rc4.encryptDecrypt(data);
        blocksEncrypted ++;
    } while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < 250);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_diff = end- start;
    unsigned long long end_cycles = __rdtsc();
    unsigned long long cycle_diff = end_cycles - start_cycles;
    double speed = static_cast<double>(cycle_diff) / data.size();
    double bits_per_second = 1.0 / (time_diff.count() / data.size());
    double speed_mbps = bits_per_second / (1024 * 1024);
    cout << "Encrypted " << blocksEncrypted << "blocks of 4096 bits (under 1 keys, " <<  blocksEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << time_diff.count() << endl;
    cout << "Encryption speed (cycles/byte):" << speed << endl;
    cout << "Encryption speed (Mbps): " << speed_mbps << endl;
}

void RC4packetEncryptionRate(int dataLength){
    vector<uint8_t> seed;
    seed.resize(18); // 128 bits key
    vector<uint8_t> data;
    data.resize(dataLength); //dataLength bytes
    for (uint8_t& byte : data) {
        byte = rand() % 256;  // Randomize the data
    }
    for (uint8_t& byte : seed) {
        byte = rand() % 256; // Randomize the data
    }
    RC4 rc4;
    rc4.keyGenerate(seed);
    int packetsEncrypted = 0;
    auto start = chrono::high_resolution_clock::now();
    unsigned long long start_cycles = __rdtsc();
    do {
        rc4.encryptDecrypt(data);
        packetsEncrypted ++;
    } while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < 250);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_diff = end- start;
    unsigned long long end_cycles = __rdtsc();
    unsigned long long cycle_diff = end_cycles - start_cycles;
    double speed = static_cast<double>(cycle_diff) / data.size();
    double bits_per_second = 1.0 / (time_diff.count() / data.size());
    double speed_mbps = bits_per_second / (1024 * 1024);
    cout << "Encrypted " << packetsEncrypted << "packets of 40 bits (under 1 keys, " <<  packetsEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << time_diff.count();
    cout << "Encryption speed (cycles/byte):" << speed << endl;
    cout << "Encryption speed (Mbps): " << speed_mbps << endl;
}

void RC4agility() {

}

void RC4keySetUpTime(){
    vector<uint8_t> seed;
    seed.resize(18);
    RC4 rc4;
    for (uint8_t& byte : seed) {
        byte = rand() % 256;  // Randomize the seed
    }
    auto start = chrono::high_resolution_clock::now();
    for(int i = 0 ; i < 7000 ; i++) {
        rc4.keyGenerate(seed);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Did 7000 key setups under 1 key" << endl;
    cout << "Key setup time: " << elapsed_seconds.count() << " seconds" << std::endl;
}


void RC4testingFrameWork(){
    display();
    cout << "Testing eStream: RC4" << endl;
    cout << "====================" << endl;
    cout << "Profile: Team 1" << endl;
    cout << "Key size: 128 bits" << endl;
    cout << endl;

    cout << "Estimated CPU frequency: " << CPUspeed() << " Hz" << endl;
    cout << "Cycles are measured using RDTSC instruction" << endl;
    cout << endl;

    cout << "Testing memory requirements:" << endl;
    cout << "Size of ECRYPT_ctx: 108 bytes" << endl;
    cout << endl;

    cout << "Testing stream encryption speed:" << endl;
    cout << endl;

    RC4encryptionRate();

    cout << "Testing packet encryption speed:" << endl;
    cout << endl;

    RC4packetEncryptionRate(40);
    cout << endl;
    RC4packetEncryptionRate(576);
    cout << endl;
    RC4packetEncryptionRate(1500);
    cout << endl;

    cout << "Testing key setup speed: " << endl;
    RC4keySetUpTime();
}

//Mickey
bool rtaps[100] = {1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1};
bool comp0[100] = {0 /*comp0[0]=X*/, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 /*comp0[100]=X*/};
bool comp1[100] = {0 /*comp1[0]=X*/, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 /*comp1[100]=X*/};
bool fb0[100] = {0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0};
bool fb1[100] = {0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0};

void clock_r(bool (&x)[100], bool ctrl_bit, bool input_bit){
    bool fb_bit = x[99]^input_bit;
    bool r_prev[100];
    for (int i = 0; i<100; i++) r_prev[i]=x[i];
    for (int i = 1; i<100; i++) x[i]=r_prev[i-1]; 
    x[0]=0;
    for (int i = 0; i<100; i++)
    if (rtaps[i]) x[i]=x[i]^fb_bit;
    if (ctrl_bit) for (int i = 0; i<100; i++) x[i]=x[i]^r_prev[i];
}

void clock_s(bool (&x)[100], bool ctrl_bit, bool input_bit){
    bool fb_bit = x[99]^input_bit;
    bool s_prev[100];
    for (int i = 0; i<100; i++) s_prev[i]=x[i];
    for (int i = 1; i<99; i++) x[i]=s_prev[i-1]^((s_prev[i]^comp0[i])&(s_prev[i+1]^comp1[i]));
    x[0]=0; x[99]=s_prev[98];
    if (!ctrl_bit) for (int i = 0; i<100; i++) x[i]=x[i]^(fb_bit&fb0[i]);
    else for (int i = 0; i<100; i++) x[i]=x[i]^(fb_bit&fb1[i]);
}

void clock_kg(bool (&x)[100], bool (&y)[100], bool mix, bool input_bit){
    bool ctrl_r = y[34]^x[67];
    bool ctrl_s = y[67]^x[33];
    bool input_r, input_s;
    if (mix) input_r=input_bit^y[50];
    else input_r=input_bit;
    input_s=input_bit;
    clock_r(x,ctrl_r,input_r);
    clock_s(y,ctrl_s,input_s);
}

void init(bool (&x)[100], bool (&y)[100], bool z[80], bool t[], int n){
    for (int i = 0; i<100; i++) {x[i]=0; y[i]=0;}
    for (int i = 0; i<n; i++) clock_kg(x,y,1,t[i]);
    for (int i = 0; i<80; i++) clock_kg(x,y,1,z[i]);
    for (int i = 0; i<100; i++) clock_kg(x,y,1,0);
}


int gcd(int a, int b){
    if (b==0) return a;
    else return gcd(b, a%b);
}

void MickeyencryptionRate(){
    bool iv[80];
    bool k[80];
    bool r[100];
    bool s[100];
    bool plaintext[4096*8];
    bool cyphertext[4096*8];
    bool keystream[4096*8];
    for (bool& bit : iv) {
        bit = rand() % 2; // Randomize the data
    }
    for (bool& bit : plaintext) {
        bit = rand() % 2; // Randomize the data
    }
    for (bool& bit : cyphertext) {
        bit = rand() % 2; // Randomize the data
    }
    for (bool& bit : k) {
        bit = rand() % 2; // Randomize the data
    }
    init(r,s,k,iv,80);
    // size_t bitsEncrypted = 0;
    int blocksEncrypted = 0;
    auto start = chrono::high_resolution_clock::now();
    // std::chrono::duration<double> time_diff;
    unsigned long long start_cycles = __rdtsc();
    do {
        for (int i = 0; i< 8 * 4096; i++) {
            keystream[i]=r[0]^s[0];
            cyphertext[i]=plaintext[i]^keystream[i];
            clock_kg(r,s,0,0);
        }
        blocksEncrypted ++;
        // bitsEncrypted += data.size() * 8; // Convert bytes to bits
        // time_diff = end_time - start_time
    } while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < 250);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_diff = end- start;
    unsigned long long end_cycles = __rdtsc();
    unsigned long long cycle_diff = end_cycles - start_cycles;
    double speed = static_cast<double>(cycle_diff) / (4096*8);
    // Calculate bits per second
    double bits_per_second = 1.0 / (time_diff.count() / (4096*8));

    // Convert bits per second to megabits per second (Mbps)
    double speed_mbps = bits_per_second / (1024 * 1024);
    cout << "Encrypted " << blocksEncrypted << "blocks of 4096 bits (under 1 keys, " <<  blocksEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << endl;
    cout << "Encryption speed (cycles/byte):" << speed << endl;
    cout << "Encryption speed (Mbps): " << speed_mbps << endl;
}

//2
void MickeypacketEncryptionRate(int dataLength){
    const int size = dataLength * 8;
    bool iv[80];
    bool k[80];
    bool r[100];
    bool s[100];
    bool plaintext[size];
    bool cyphertext[size];
    bool keystream[size];
    for (bool& bit : iv) {
        bit = rand() % 2; // Randomize the data
    }
    for (bool& bit : plaintext) {
        bit = rand() % 2; // Randomize the data
    }
    for (bool& bit : cyphertext) {
        bit = rand() % 2; // Randomize the data
    }
    for (bool& bit : k) {
        bit = rand() % 2; // Randomize the data
    }
    init(r,s,k,iv,80);
    // size_t bitsEncrypted = 0;
    int packetsEncrypted = 0;
    auto start = chrono::high_resolution_clock::now();
    unsigned long long start_cycles = __rdtsc();
    do {
        for (int i = 0; i< 8 * dataLength; i++) {
            keystream[i]=r[0]^s[0];
            cyphertext[i]=plaintext[i]^keystream[i];
            clock_kg(r,s,0,0);
        }
        packetsEncrypted ++;
        // bitsEncrypted += data.size() * 8; // Convert bytes to bits
        // time_diff = end_time - start_time
    } while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < 250);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_diff = end- start;
    unsigned long long end_cycles = __rdtsc();
    unsigned long long cycle_diff = end_cycles - start_cycles;
    double speed = static_cast<double>(cycle_diff) / dataLength;
    // Calculate bits per second
    double bits_per_second = 1.0 / (time_diff.count() / dataLength);

    // Convert bits per second to megabits per second (Mbps)
    double speed_mbps = bits_per_second / (1024 * 1024);
    cout << "Encrypted " << packetsEncrypted << "packets of 40 bits (under 1 keys, " <<  packetsEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << time_diff.count();
    cout << "Encryption speed (cycles/byte):" << speed << endl;
    cout << "Encryption speed (Mbps): " << speed_mbps << endl;

}

void Mickeyagility() {

}

void MickeykeySetUpTime(){
    bool iv[80];
    bool k[80];
    bool r[100];
    bool s[100];
    bool plaintext[40 /*Change this*/];
    bool cyphertext[40 /*Change this*/];
    bool keystream[40 /*Change this*/];
    for (bool& bit : iv) {
        bit = rand() % 2; // Randomize the data
    }
    for (bool& bit : k) {
        bit = rand() % 2; // Randomize the data
    }
    init(r,s,k,iv,80);
    auto start = chrono::high_resolution_clock::now();
    for(int i = 0 ; i < 7000 ; i++) {
        init(r,s,k,iv,80);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Did 7000 key setups under 1 key" << endl;
    cout << "Keystream setup time: " << elapsed_seconds.count() << " seconds" << std::endl;
}


void MickeytestingFrameWork(){
    display();
    cout << "Testing eStream: Mickey2.0>" << endl;
    cout << "====================" << endl;
    cout << "Profile: Team 1" << endl;
    cout << "Key size: 80 bits" << endl;
    cout << "IV size: 80 bits" << endl ;
    cout << endl;

    cout << "Estimated CPU frequency: " << CPUspeed() << " Hz" << endl;
    cout << "Cycles are measured using RDTSC instruction" << endl;
    cout << endl;

    cout << "Testing memory requirements:" << endl;
    cout << "Size of ECRYPT_ctx: 108 bytes" << endl;
    cout << endl;

    cout << "Testing stream encryption speed:" << endl;
    cout << endl;

    MickeyencryptionRate();

    cout << "Testing packet encryption speed:" << endl;
    cout << endl;

    MickeypacketEncryptionRate(40);
    cout << endl;
    MickeypacketEncryptionRate(576);
    cout << endl;
    MickeypacketEncryptionRate(1500);
    cout << endl;

    cout << "Testing key setup speed: " << endl;
    MickeykeySetUpTime();
}


void display() {
    cout << "Welcome to the encryption testing site!" << endl;
    cout << "In this testing site, we are gonna have four different tests for an eStream:" << endl;
    cout << "1. Encryption rate for long streams: The testing framework measures the encryption rate by encrypting a long stream in chunks of about 4KB. The encryption speed, in cycles/byte, is calculated by measuring the number of bytes encrypted in 250 µsec." << endl;
    cout << "!) Note that the time to setup the key or the IV is not considered in this test." << endl;
    cout << "2. Packet encryption rate: While a block cipher is likely to be a better choice when encrypting very short packets, it is still interesting to determine at which length a stream cipher starts to take the lead. " << endl;
    cout << "The packet encryption rate is measured by applying with the packets of different lengths. Each call includes a separate IV setup and and the packet lengths (40, 576, and 1500 bytes) were chosen to be representative for the traffic seen on the Internet [JTC-003]." << endl;
    cout << "3. Agility : The testing framework performs the following test: it first initiates a large number of sessions (filling 16MB of RAM ), and then encrypts streams of plaintext in short blocks of around 256 bytes" << endl;
    cout << "4. Key and IV setup: The last test in the testing framework separately measures the efficiency of the key setup and the IV setup" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
}

int main() {
    display();
    RC4testingFrameWork();
    cout << endl << endl;
    MickeytestingFrameWork();
    cout << "End of performance measurements";
}