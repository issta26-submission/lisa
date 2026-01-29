#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // We include required headers here so the function is self-contained.
    // (Including inside a function is allowed; the preprocessor runs before compilation.)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>

    // Helper lambda for a quick failure cleanup/return
    auto fail = [](int code) -> int { return code; };

    // Sample input data
    std::string input = "The quick brown fox jumps over the lazy dog. "
                        "This is a test of the zlib API sequence. "
                        "We will use compress2/uncompress, streaming deflate/inflate, and gz* APIs.";
    // Make the data a bit larger and repetitive so compression actually does something
    for (int i = 0; i < 4; ++i) input += input;

    // 1) Simple one-shot compress2 + uncompress
    {
        uLong sourceLen = static_cast<uLong>(input.size());
        uLong destLen = compressBound(sourceLen);
        std::vector<unsigned char> comp(destLen);

        int cres = compress2(comp.data(), &destLen,
                             reinterpret_cast<const Bytef*>(input.data()),
                             sourceLen, Z_BEST_COMPRESSION);
        if (cres != Z_OK) return fail(1);
        comp.resize(destLen);

        // Decompress
        std::vector<unsigned char> decomp(sourceLen);
        uLong decompressedLen = sourceLen;
        int ures = uncompress(decomp.data(), &decompressedLen,
                              comp.data(), comp.size());
        if (ures != Z_OK) return fail(2);
        if (decompressedLen != sourceLen) return fail(3);
        if (!std::equal(decomp.begin(), decomp.end(), input.begin()))
            return fail(4);
    }

    // 2) Streaming deflate (using z_stream) and streaming inflate to verify result
    std::vector<unsigned char> streamCompressed;
    {
        z_stream def;
        std::memset(&def, 0, sizeof(def));
        if (deflateInit(&def, Z_DEFAULT_COMPRESSION) != Z_OK) return fail(5);

        def.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
        def.avail_in = static_cast<uInt>(input.size());

        const std::size_t CHUNK = 128;
        unsigned char outbuf[CHUNK];

        int ret;
        do {
            def.next_out = outbuf;
            def.avail_out = static_cast<uInt>(CHUNK);

            ret = deflate(&def, Z_FINISH); // we provide all input at once, signal finish
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&def);
                return fail(6);
            }
            std::size_t have = CHUNK - def.avail_out;
            streamCompressed.insert(streamCompressed.end(), outbuf, outbuf + have);
        } while (ret != Z_STREAM_END);

        deflateEnd(&def);
    }

    // Inflate the streaming-compressed data
    std::vector<unsigned char> streamDecompressed;
    {
        z_stream inf;
        std::memset(&inf, 0, sizeof(inf));
        if (inflateInit(&inf) != Z_OK) return fail(7);

        inf.next_in = streamCompressed.empty() ? nullptr : streamCompressed.data();
        inf.avail_in = static_cast<uInt>(streamCompressed.size());

        const std::size_t CHUNK = 128;
        unsigned char outbuf[CHUNK];

        int ret;
        do {
            inf.next_out = outbuf;
            inf.avail_out = static_cast<uInt>(CHUNK);

            ret = inflate(&inf, Z_NO_FLUSH);
            if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                inflateEnd(&inf);
                return fail(8);
            }
            std::size_t have = CHUNK - inf.avail_out;
            streamDecompressed.insert(streamDecompressed.end(), outbuf, outbuf + have);
        } while (ret != Z_STREAM_END);

        inflateEnd(&inf);

        if (streamDecompressed.size() != input.size()) return fail(9);
        if (!std::equal(streamDecompressed.begin(), streamDecompressed.end(), input.begin()))
            return fail(10);
    }

    // 3) gz* APIs: write the original input to a .gz file, read it back and verify
    const char* fname = "test_zlib_api_sequence_tmp.gz";
    {
        gzFile gz = gzopen(fname, "wb");
        if (!gz) return fail(11);
        unsigned int toWrite = static_cast<unsigned int>(input.size());
        int written = gzwrite(gz, input.data(), toWrite);
        if (written != static_cast<int>(toWrite)) {
            gzclose(gz);
            std::remove(fname);
            return fail(12);
        }
        gzclose(gz);
    }
    {
        gzFile gz = gzopen(fname, "rb");
        if (!gz) {
            std::remove(fname);
            return fail(13);
        }
        std::vector<char> readbuf;
        const std::size_t CHUNK = 128;
        char out[CHUNK];
        int r;
        while ((r = gzread(gz, out, static_cast<unsigned int>(CHUNK))) > 0) {
            readbuf.insert(readbuf.end(), out, out + r);
        }
        if (r < 0) { // error during read
            gzclose(gz);
            std::remove(fname);
            return fail(14);
        }
        gzclose(gz);

        if (readbuf.size() != input.size()) {
            std::remove(fname);
            return fail(15);
        }
        if (!std::equal(readbuf.begin(), readbuf.end(), input.begin())) {
            std::remove(fname);
            return fail(16);
        }

        // Clean up the temporary file
        std::remove(fname);
    }

    // If all tests passed, return 66 as requested
    return 66;
}