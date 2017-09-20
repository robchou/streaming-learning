#ifndef WAV_FILE_FORMAT_H_
#define WAV_FILE_FORMAT_H_

#include <cstring>
#include <cinttypes>

/**
 * @brief The canonical WAVE format starts with the RIFF header
 */
struct RiffChunk {
    // Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form).
    char chunk_id[4];

    // 36 + SubChunk2Size, or more precisely:
    // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    // This is the size of the rest of the chunk following this number.
    // This is the size of the entire file in bytes minus 8 bytes for the
    // two fields not included in this count: ChunkID and ChunkSize.
    uint32_t chunk_size;

    //Contains the letters "WAVE" (0x57415645 big-endian form)
    char format[4];

    RiffChunk() {
        memcpy(chunk_id, "RIFF", 4);
        chunk_size = 0;
        memcpy(format, "WAVE", 4);
    }
};

/**
 * @brief The "WAVE" format consists of two subchunks: "fmt " and "data":
 * The "fmt " subchunk describes the sound data's format:
 */
struct FormatChunk {
    // Contains the letters "fmt " (0x666d7420 big-endian form).
    char chunk_id[4];
    // 16 for PCM.
    // This is the size of the rest of the Subchunk which follows this number.
    uint32_t chunk_size;
    // PCM = 1 (i.e. Linear quantization)
    // Values other than 1 indicate some form of compression.
    uint16_t audio_format;
    // Mono = 1, Stereo = 2, etc.
    uint16_t num_channels;
    // 8000, 44100, etc.
    uint32_t sample_rate;
    // SampleRate * NumChannels * BitsPerSample / 8
    uint32_t byte_rate;
    // NumChannels * BitsPerSample / 8
    uint16_t block_align;
    // The number of bytes for one sample including all channels.
    // I wonder what happens when this number isn't an integer?
    uint16_t bits_per_sample;
    // if PCM, then doesn't exist
    // uint16_t extra_param_size;
    // space for extra parameters
    // uintx_t extra_params;
    FormatChunk() {
        // Default: pcm s16le 2 channels 44.1k
        memcpy(chunk_id, "fmt ", 4);
        chunk_size = 16;
        audio_format = 1;
        num_channels = 2;
        sample_rate = 44100;
        byte_rate = sample_rate * num_channels * 16 / 8;
        block_align = num_channels * 16 / 8;
        bits_per_sample = 16;
    }
};

/**
 * @brief The "data" subchunk contains the size of the data and the actual sound:
 */
struct DataChunk {
    // Contains the letters "data" (0x64617461 big-endian form).
    char chunk_id[4];
    // NumSamples * NumChannels * BitsPerSample / 8
    // This is the number of bytes in the data.
    // You can also think of this as the size
    // of the read of the subchunk following this number.
    uint32_t chunk_size;
    // The actual sound data.
    // uint8_t *data;
    DataChunk() {
        memcpy(chunk_id, "data", 4);
        chunk_size = 0;
    }
};

#endif // WAV_FILE_FORMAT_H_
