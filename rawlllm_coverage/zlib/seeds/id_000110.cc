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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Requires zlib headers and library at compile/link time:
    // #include <zlib.h>
    // #include <vector>
    // #include <string>
    // #include <cstring>
    // #include <cstdio>
    // #include <cstdlib>
    // The includes are assumed available in the translation unit that contains this function.

    // Prepare sample data
    const std::string input = std::string("The quick brown fox jumps over the lazy dog. ") + 
                              "0123456789abcdef0123456789abcdef";
    const size_t inputLen = input.size();

    // 1) Compute checksums: adler32 and crc32
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(inputLen));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(inputLen));

    // Basic sanity: both checksums should be non-zero for non-empty data
    if (adl == 0 || crc == 0) return -1;

    // 2) compress2 + uncompress (one-shot)
    uLong bound = compressBound(static_cast<uLong>(inputLen));
    std::vector<Bytef> compBuf(bound);
    uLongf compSize = static_cast<uLongf>(bound);
    int zret = compress2(compBuf.data(), &compSize,
                         reinterpret_cast<const Bytef*>(input.data()),
                         static_cast<uLong>(inputLen),
                         Z_BEST_COMPRESSION);
    if (zret != Z_OK) return -2;
    compBuf.resize(compSize);

    std::vector<Bytef> uncompBuf(inputLen);
    uLongf uncmpSize = static_cast<uLongf>(inputLen);
    zret = uncompress(uncompBuf.data(), &uncmpSize, compBuf.data(), compSize);
    if (zret != Z_OK) return -3;
    if (uncmpSize != inputLen) return -4;
    if (memcmp(uncompBuf.data(), input.data(), inputLen) != 0) return -5;

    // 3) Streaming deflate/inflate using z_stream
    {
        // Deflate (compress) streaming
        z_stream defstrm;
        memset(&defstrm, 0, sizeof(defstrm));
        if (deflateInit(&defstrm, Z_DEFAULT_COMPRESSION) != Z_OK) return -6;

        const size_t CHUNK = 16384;
        std::vector<Bytef> streamComp;
        streamComp.reserve(compSize); // heuristic

        size_t readPos = 0;
        int defRet = Z_OK;
        while (defRet != Z_STREAM_END) {
            uInt have_out;
            if (readPos < inputLen) {
                uInt give = static_cast<uInt>(std::min<size_t>(CHUNK, inputLen - readPos));
                defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data() + readPos));
                defstrm.avail_in = give;
                readPos += give;
            } else {
                defstrm.next_in = Z_NULL;
                defstrm.avail_in = 0;
            }
            int flush = (readPos >= inputLen) ? Z_FINISH : Z_NO_FLUSH;
            do {
                // prepare output
                streamComp.resize(streamComp.size() + CHUNK);
                defstrm.next_out = streamComp.data() + (streamComp.size() - CHUNK);
                defstrm.avail_out = CHUNK;
                defRet = deflate(&defstrm, flush);
                if (defRet == Z_STREAM_ERROR) {
                    deflateEnd(&defstrm);
                    return -7;
                }
                have_out = CHUNK - defstrm.avail_out;
                streamComp.resize((streamComp.size() - CHUNK) + have_out);
            } while (defstrm.avail_out == 0);
        }
        deflateEnd(&defstrm);

        // Inflate (decompress) streaming
        z_stream infstrm;
        memset(&infstrm, 0, sizeof(infstrm));
        if (inflateInit(&infstrm) != Z_OK) return -8;

        std::vector<Bytef> streamUncomp;
        streamUncomp.reserve(inputLen);

        size_t cpos = 0;
        int infRet = Z_OK;
        while (infRet != Z_STREAM_END) {
            if (cpos < streamComp.size()) {
                uInt give = static_cast<uInt>(std::min<size_t>(CHUNK, streamComp.size() - cpos));
                infstrm.next_in = streamComp.data() + cpos;
                infstrm.avail_in = give;
                cpos += give;
            } else {
                infstrm.next_in = Z_NULL;
                infstrm.avail_in = 0;
            }
            do {
                streamUncomp.resize(streamUncomp.size() + CHUNK);
                infstrm.next_out = streamUncomp.data() + (streamUncomp.size() - CHUNK);
                infstrm.avail_out = CHUNK;
                infRet = inflate(&infstrm, Z_NO_FLUSH);
                if (infRet == Z_NEED_DICT || infRet == Z_DATA_ERROR || infRet == Z_MEM_ERROR) {
                    inflateEnd(&infstrm);
                    return -9;
                }
                uInt have = CHUNK - infstrm.avail_out;
                streamUncomp.resize((streamUncomp.size() - CHUNK) + have);
            } while (infstrm.avail_out == 0);
            if (cpos >= streamComp.size() && infstrm.avail_in == 0 && infRet != Z_STREAM_END) {
                // No more input left; if not finished, break to avoid infinite loop
                if (infRet != Z_STREAM_END) {
                    inflateEnd(&infstrm);
                    return -10;
                }
            }
        }
        inflateEnd(&infstrm);

        if (streamUncomp.size() != inputLen) return -11;
        if (memcmp(streamUncomp.data(), input.data(), inputLen) != 0) return -12;
    }

    // 4) Use gz* convenience functions to write/read a .gz file
    {
        const char *fname = "test_zlib_api_sequence_tmp.gz";
        // gzopen for writing (binary)
        gzFile gz = gzopen(fname, "wb");
        if (!gz) return -13;
        // Optionally set internal buffer
        gzbuffer(gz, 8192);
        // gzwrite returns number of uncompressed bytes written
        int written = gzwrite(gz, input.data(), static_cast<unsigned int>(inputLen));
        if (written != static_cast<int>(inputLen)) {
            gzclose(gz);
            remove(fname);
            return -14;
        }
        if (gzclose(gz) != Z_OK) {
            remove(fname);
            return -15;
        }

        // gzopen for reading
        gz = gzopen(fname, "rb");
        if (!gz) {
            remove(fname);
            return -16;
        }
        std::vector<char> fileBuf(inputLen + 16); // a bit of slack
        int readBytes = gzread(gz, fileBuf.data(), static_cast<unsigned int>(fileBuf.size()));
        if (readBytes < 0) {
            gzclose(gz);
            remove(fname);
            return -17;
        }
        if (gzclose(gz) != Z_OK) {
            remove(fname);
            return -18;
        }
        // remove temporary file
        remove(fname);

        if (static_cast<size_t>(readBytes) != inputLen) return -19;
        if (memcmp(fileBuf.data(), input.data(), inputLen) != 0) return -20;
    }

    // If all checks passed, return 66 as requested
    return 66;
}