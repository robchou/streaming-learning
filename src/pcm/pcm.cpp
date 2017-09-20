#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "wav.h"

using std::string;

/**
 * @brief Convert pcm data to wav.
 *
 * @param src pcm file path
 * @param dest output wav file path
 * @param sample_rate sample rate
 * @param bits bits per sample
 * @param channels channels
 */
void pcm2wav(const string& src, const string& dest, int sample_rate,
        int bits, int channels) {
    RiffChunk riff;
    FormatChunk fmt;
    DataChunk data;

    std::ifstream in(src, std::ios::ate | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "error: open " << src << " failed!" << std::endl;
        return;
    }

    std::ofstream out(dest, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "error: open " << dest << " failed!" << std::endl;
        return;
    }

    data.chunk_size = in.tellg();

    fmt.num_channels = channels;
    fmt.sample_rate = sample_rate;
    fmt.byte_rate = sample_rate * channels * bits / 8;
    fmt.block_align = channels * bits / 8;
    fmt.bits_per_sample = bits;

    riff.chunk_size = 36 + data.chunk_size;

    // write wav file header
    out.write(reinterpret_cast<char*>(&riff), sizeof(RiffChunk));
    out.write(reinterpret_cast<char*>(&fmt), sizeof(FormatChunk));
    out.write(reinterpret_cast<char*>(&data), sizeof(DataChunk));

    char* pcm_data = new char [data.chunk_size];
    in.seekg(0, std::ios::beg);
    in.read(pcm_data, data.chunk_size);

    // write pcm data
    out.write(pcm_data, data.chunk_size);

    if (in.is_open()) {
        in.close();
    }

    if (out.is_open()) {
        out.close();
    }
}

/**
 * @brief split PCM stero s16le 44100 into 2 channels
 *
 * @param src PCM file path
 * @param left channel1 output file
 * @param right channel2 output file
 */
void split(const string& src, const string& left, const string& right) {
    std::ifstream in(src, std::ios::in | std::ios::binary);
    std::ofstream channel1(left, std::ios::out | std::ios::trunc | std::ios::binary);
    std::ofstream channel2(right, std::ios::out | std::ios::trunc | std::ios::binary);

    if (!in.is_open()) {
        std::cerr << "error: open " << src << " failed!" << std::endl;
        return;
    }

    if (!channel1.is_open()) {
        std::cerr << "error: open " << left << " failed!" << std::endl;
        return;
    }

    if (!channel2.is_open()) {
        std::cerr << "error: open " << right << " failed!" << std::endl;
        return;
    }

    char sample[4];

    while (!in.eof()) {
        in.read(sample, 4);
        channel1.write(sample, 2);
        channel2.write(sample + 2, 2);
    }

    if (in.is_open()) {
        in.close();
    }

    if (channel1.is_open()) {
        channel1.close();
    }

    if (channel2.is_open()) {
        channel2.close();
    }
}

/**
 * @brief Half the volume of the left channel
 *
 * @param src PCM file path
 * @param dest output PCM file path
 */
void volume(const string& src, const string& dest) {
    std::ifstream in(src, std::ios::in | std::ios::binary);
    std::ofstream out(dest, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!in.is_open()) {
        std::cerr << "error: open " << src << " failed!" << std::endl;
        return;
    }

    if (!out.is_open()) {
        std::cerr << "error: open " << dest << " failed!" << std::endl;
        return;
    }

    char sample[4];

    while (!in.eof()) {
        in.read(sample, 4);
        int16_t* samplenum = reinterpret_cast<int16_t*>(sample);
        *samplenum = *samplenum / 2;
        out.write(sample, 4);
    }

    if (in.is_open()) {
        in.close();
    }

    if (out.is_open()) {
        out.close();
    }
}

/**
 * @brief 2x speed to play
 *
 * @param src PCM file path
 * @param dest output PCM file path
 */
void speed(const string& src, const string& dest) {
    std::ifstream in(src, std::ios::in | std::ios::binary);
    std::ofstream out(dest, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!in.is_open()) {
        std::cerr << "error: open " << src << " failed!" << std::endl;
        return;
    }

    if (!out.is_open()) {
        std::cerr << "error: open " << dest << " failed!" << std::endl;
        return;
    }

    char sample[4];
    long cnt = 0;

    while (!in.eof()) {
        in.read(sample, 4);
        if (cnt % 2 == 0) {
            out.write(sample, 4);
        }
        ++cnt;
    }

    if (in.is_open()) {
        in.close();
    }

    if (out.is_open()) {
        out.close();
    }
}

int main(int argc, char *argv[])
{
    pcm2wav("../NocturneNo2inEflat_44.1k_s16le.pcm", "pcm2wav.wav", 44100, 16, 2);
    split("../NocturneNo2inEflat_44.1k_s16le.pcm", "channel1.pcm", "channel2.pcm");
    volume("../NocturneNo2inEflat_44.1k_s16le.pcm", "volume.pcm");
    speed("../NocturneNo2inEflat_44.1k_s16le.pcm", "speed.pcm");
    return 0;
}
