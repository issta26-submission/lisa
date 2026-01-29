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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained for embedding in a source file.
    // Preprocessor directives are allowed here; they will be processed before compilation.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Sample input data
    const std::string original = std::string("The quick brown fox jumps over the lazy dog. ")
                                 + std::string("This is a longer test string to exercise zlib APIs. ");
    // Repeat to increase size
    std::string input;
    for (int i = 0; i < 50; ++i) input += original;

    // 1) Use compress() / uncompress() (simple single-shot API)
    {
        uLong sourceLen = static_cast<uLong>(input.size());
        uLong bound = compressBound(sourceLen);
        std::vector<unsigned char> compBuf(bound);

        uLong destLen = bound;
        int cres = compress(compBuf.data(), &destLen,
                            reinterpret_cast<const Bytef*>(input.data()), sourceLen);
        if (cres != Z_OK) return 1; // compress() failed

        // Resize to actual compressed size
        compBuf.resize(destLen);

        std::vector<unsigned char> decompBuf(sourceLen);
        uLong decompLen = sourceLen;
        int ures = uncompress(decompBuf.data(), &decompLen,
                              compBuf.data(), destLen);
        if (ures != Z_OK) return 2; // uncompress() failed

        if (decompLen != sourceLen) return 3; // size mismatch
        if (std::memcmp(decompBuf.data(), input.data(), sourceLen) != 0) return 4; // data mismatch
    }

    // 2) Compute checksums (crc32 and adler32)
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before,
                       reinterpret_cast<const Bytef*>(input.data()),
                       static_cast<uInt>(input.size()));
    uLong adler_before = adler32(0L,
                                 reinterpret_cast<const Bytef*>(input.data()),
                                 static_cast<uInt>(input.size()));

    // 3) Use low-level streaming deflate/inflate APIs (deflateInit/deflate/deflateEnd and inflateInit/inflate/...)
    std::vector<unsigned char> deflated;
    deflated.reserve(1024);

    {
        z_stream defstrm;
        std::memset(&defstrm, 0, sizeof(defstrm));
        int ret = deflateInit(&defstrm, Z_BEST_COMPRESSION);
        if (ret != Z_OK) return 10;

        defstrm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
        defstrm.avail_in = static_cast<uInt>(input.size());

        const size_t OUT_CHUNK = 16384;
        std::vector<unsigned char> outChunk(OUT_CHUNK);

        // deflate until the input is consumed and stream finished
        int flush = Z_FINISH;
        do {
            defstrm.next_out = outChunk.data();
            defstrm.avail_out = static_cast<uInt>(outChunk.size());
            ret = deflate(&defstrm, flush);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&defstrm);
                return 11;
            }
            size_t have = outChunk.size() - defstrm.avail_out;
            deflated.insert(deflated.end(), outChunk.data(), outChunk.data() + have);
        } while (ret != Z_STREAM_END);

        deflateEnd(&defstrm);
    }

    // 4) Inflate the deflated data and verify equality
    {
        z_stream instrm;
        std::memset(&instrm, 0, sizeof(instrm));
        int ret = inflateInit(&instrm);
        if (ret != Z_OK) return 20;

        instrm.next_in = deflated.empty() ? Z_NULL : deflated.data();
        instrm.avail_in = static_cast<uInt>(deflated.size());

        const size_t OUT_CHUNK = 16384;
        std::vector<unsigned char> outChunk(OUT_CHUNK);
        std::vector<unsigned char> inflated;
        inflated.reserve(input.size());

        do {
            instrm.next_out = outChunk.data();
            instrm.avail_out = static_cast<uInt>(outChunk.size());
            ret = inflate(&instrm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&instrm);
                return 21;
            }
            size_t have = outChunk.size() - instrm.avail_out;
            inflated.insert(inflated.end(), outChunk.data(), outChunk.data() + have);
        } while (ret != Z_STREAM_END);

        inflateEnd(&instrm);

        if (inflated.size() != input.size()) return 22;
        if (std::memcmp(inflated.data(), input.data(), input.size()) != 0) return 23;

        // Compute checksums on inflated data and compare
        uLong crc_after = crc32(0L, Z_NULL, 0);
        crc_after = crc32(crc_after, inflated.data(), static_cast<uInt>(inflated.size()));
        if (crc_after != crc_before) return 24;

        uLong adler_after = adler32(0L, inflated.data(), static_cast<uInt>(inflated.size()));
        if (adler_after != adler_before) return 25;
    }

    // All tests succeeded
    return 66;
}