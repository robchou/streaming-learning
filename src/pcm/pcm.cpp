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
void pcm2wav(const string& src, const string &dest, int sample_rate,
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

int main(int argc, char *argv[])
{
    pcm2wav("../NocturneNo2inEflat_44.1k_s16le.pcm", "pcm2wav.wav", 44100, 16, 2);
    return 0;
}
