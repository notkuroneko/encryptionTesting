#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono> // For timing
#include <thread>
#include <fstream>
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
    seed.resize(16); // 128 bits key
    vector<uint8_t> data;
    data.resize(4096); // Resize the data to 4kB.
    for (uint8_t& byte : data) {
        byte = rand() % 256; // Randomize the data
    }
    for (uint8_t& byte : seed) {
        byte = rand() % 256; // Randomize the data
    }
    RC4 rc4;
    rc4.keyGenerate(seed);
    size_t bitsEncrypted = 0;
    int blocksEncrypted = 0;
    auto start = chrono::high_resolution_clock::now();
    unsigned long long start_cycles = __rdtsc();
    // std::chrono::duration<double> time_diff;
    do {
        rc4.encryptDecrypt(data);
        blocksEncrypted ++;
        bitsEncrypted += data.size() * 8; // Convert bytes to bits
        // time_diff = end_time - start_time
    } while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < 250);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_diff = end- start;
    unsigned long long end_cycles = __rdtsc();
    unsigned long long cycle_diff = end_cycles - start_cycles;
    double speed = static_cast<double>(cycle_diff) / data.size();
    // Calculate bits per second
    double bits_per_second = 1.0 / (time_diff.count() / data.size());

    // Convert bits per second to megabits per second (Mbps)
    double speed_mbps = bits_per_second / (1024 * 1024);
    cout << "Encrypted " << blocksEncrypted << "blocks of 4096 bits (under 1 keys, " <<  blocksEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << time_diff.count() << endl;
    cout << "Encryption speed (cycles/byte):" << speed << endl;
    cout << "Encryption speed (Mbps): " << speed_mbps << endl;
}

//2
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
        // time_diff = end_time - start_time
    } while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < 250);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_diff = end- start;
    unsigned long long end_cycles = __rdtsc();
    unsigned long long cycle_diff = end_cycles - start_cycles;
    double speed = static_cast<double>(cycle_diff) / data.size();
    // Calculate bits per second
    double bits_per_second = 1.0 / (time_diff.count() / data.size());

    // Convert bits per second to megabits per second (Mbps)
    double speed_mbps = bits_per_second / (1024 * 1024);
    cout << "Encrypted " << packetsEncrypted << "packets of 40 bits (under 1 keys, " <<  packetsEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << time_diff.count() << endl;
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

void RC4testingFrameWork(){
    //display();
    cout << "Testing eStream: RC4" << endl;
    cout << "====================" << endl;
    cout << "Profile: Team 1" << endl;
    cout << "Key size:" << endl;
    cout << "IV size:" << endl ;
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

//Mickey           0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 
bool rtaps[100] = {1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0};

//Mickey           0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 
bool comp0[100] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0 /*comp0[0,99]=X*/};
bool comp1[100] = {0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0 /*comp1[0,99]=X*/};
bool fb0[100]   = {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0};
bool fb1[100]   = {1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

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
    cout << "Encrypted " << blocksEncrypted << " blocks of 4096 bits (under 1 keys, " <<  blocksEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << time_diff.count() << endl;
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
        // time_diff = end_time - start_time (does not include time taken to initialize regisers)
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
    cout << "Encrypted " << packetsEncrypted << " packets of 40 bits (under 1 keys, " <<  packetsEncrypted << " blocks/key)" << endl;
    cout << "Total time: " << time_diff.count() << endl;
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
    cout << "Did 7000 key iniializations under 1 key" << endl;
    cout << "Keystream initialization time: " << elapsed_seconds.count() << " seconds" << std::endl;
}


void MickeytestingFrameWork(){
    //display();
    cout << "Testing eStream: Mickey2.0>" << endl;
    cout << "====================" << endl;
    cout << "Profile: Team 1" << endl;
    cout << "Key size: 80" << endl;
    cout << "IV size: 80" << endl ;
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

void read_input(bool (&x)[80], bool (&y)[80], int &n){ 
    string s; int count;
    ifstream f;
    f.open("mickey_key");
    getline(f, s);
    f.close();
    count = 0;
    for (int i=0; i<s.length(); i++) 
    switch (s[i]) {
        case '0': {x[count]=0; count++; break;} 
        case '1': {x[count]=1; count++; break;} 
        default: {break;}
    }
    if (count!=80) cout << "Key doesn't have sufficient length";
    f.open("mickey_iv");
    getline(f, s);
    f.close();
    count = 0;
    for (int i=0; i<s.length(); i++) 
    switch (s[i]) {
        case '0': {y[count]=0; count++; break;} 
        case '1': {y[count]=1; count++; break;} 
        default: {break;}
    }
    if (count>80) cout << "Init vector is too long";
    n = count;
}

void mickey(string input_str){
    ofstream f;
    f.open("readme.md", ios::out | ios::trunc);
    f.close();
    bool iv[80];
    bool k[80];
    bool r[100];
    bool s[100];
    int iv_length;
    read_input(k,iv,iv_length);
    int temp, count;
    bool bit_stream[8*input_str.length()];
    for (int i=0; i<input_str.length(); i++) {
        temp = (int)input_str[i];
        count = 8;
        while (count) {
            count--;
            bit_stream[8*i+count]=temp%2;
            temp/=2;
        }
    }
    bool cyphertext[8*(input_str.length()+1)];
    bool keystream[8*(input_str.length()+1)];
    init(r,s,k,iv,iv_length);
    for (int i = 0; i<8*(input_str.length()+1); i++) {
        keystream[i]=r[0]^s[0];
        cyphertext[i]=bit_stream[i]^keystream[i];
        clock_kg(r,s,0,0);
    }
    f.open("readme.md");
    for (int i = 0; i<8*(input_str.length()+1); i++) {if (!(i%4)) f << " "; f << cyphertext[i];}
    f.close();
    cout << "Cyphertext transfered to readme.md" << endl;
}

void RC4_interface(string input_str){
    ofstream f;
    f.open("readme.md", ios::out | ios::trunc);
    f.close();
    int size, count; 
    vector<uint8_t> seed;
    vector<uint8_t> data;
    cout << "Enter seed byte size: ";
    cin >> size;
    seed.resize(size);
    int temp;
    for (uint8_t& byte : seed) {
        cout << "Enter next seed byte (0 to 255): ";
        cin >> temp;
        byte = temp;
    }
    data.resize(input_str.length()); count = 0;
    for (uint8_t& byte : data) {byte = (int)input_str[count]; count++;}
    RC4 rc4;
    rc4.keyGenerate(seed);
    rc4.encryptDecrypt(data);
    f.open("readme.md");
    for (uint8_t& byte : data) {
        bool b[8];
        temp = (int)byte;
        count = 8;
        while (count) {
            count--;
            b[count]=temp%2;
            temp/=2;
        }
        f << b[0] << b[1] << b[2] << b[3] <<  " " << b[4] << b[5] << b[6] << b[7] <<  " "; 
    }
    f.close();
    cout << "Cyphertext transfered to readme.md" << endl;
}

int main() {    
    display();
    RC4testingFrameWork();
    cout << endl << endl;
    MickeytestingFrameWork();
    cout << "End of performance measurements" << endl << endl;

    string p,k;
    int method;
    cout << "Enter plaintext: " << endl;
    getline(cin, p);
    cout << "Enter method of encode (0 for RC4, 1 for Mickey): ";
    cin >> method;
    if (method==0) RC4_interface(p);
    else if (method==1) mickey(p);
    return 0;
}