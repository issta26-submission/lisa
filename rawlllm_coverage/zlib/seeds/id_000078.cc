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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress / inflate / gzip
    const std::string original = "The quick brown fox jumps over the lazy dog. "
                                 "This is a test of the zlib API sequence. "
                                 "Repeat: The quick brown fox jumps over the lazy dog.";

    // 1) Simple one-shot compress() / uncompress()
    {
        uLongf srcLen = static_cast<uLongf>(original.size());
        uLongf bound = compressBound(srcLen);
        std::vector<Bytef> comp(bound);

        int ret = compress(comp.data(), &bound,
                           reinterpret_cast<const Bytef*>(original.data()),
                           srcLen);
        if (ret != Z_OK) return -1;
        comp.resize(bound); // actual compressed size

        // uncompress into buffer
        std::vector<Bytef> uncmp(srcLen);
        uLongf uncmpLen = srcLen;
        ret = uncompress(uncmp.data(), &uncmpLen, comp.data(), static_cast<uLongf>(comp.size()));
        if (ret != Z_OK) return -2;
        if (uncmpLen != srcLen) return -3;
        if (std::memcmp(uncmp.data(), original.data(), srcLen) != 0) return -4;
    }

    // 2) Streaming deflate / inflate using z_stream
    std::vector<unsigned char> streamCompressed;
    {
        z_stream strm;
        std::memset(&strm, 0, sizeof(strm));
        int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return -5;

        const Bytef* in = reinterpret_cast<const Bytef*>(original.data());
        strm.next_in = const_cast<Bytef*>(in); // zlib API is C-style; const_cast used for API
        strm.avail_in = static_cast<uInt>(original.size());

        const size_t CHUNK = 128;
        std::vector<Bytef> out(CHUNK);

        // Use Z_FINISH in a loop until Z_STREAM_END
        do {
            strm.next_out = out.data();
            strm.avail_out = static_cast<uInt>(CHUNK);
            ret = deflate(&strm, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&strm);
                return -6;
            }
            size_t have = CHUNK - strm.avail_out;
            streamCompressed.insert(streamCompressed.end(), out.data(), out.data() + have);
        } while (ret != Z_STREAM_END);

        deflateEnd(&strm);
    }

    // Inflate the streaming-compressed data and verify matches original
    {
        z_stream strm;
        std::memset(&strm, 0, sizeof(strm));
        int ret = inflateInit(&strm);
        if (ret != Z_OK) return -7;

        strm.next_in = streamCompressed.empty() ? Z_NULL : streamCompressed.data();
        strm.avail_in = static_cast<uInt>(streamCompressed.size());

        const size_t CHUNK = 128;
        std::vector<Bytef> out(CHUNK);
        std::vector<unsigned char> decompressed;
        do {
            strm.next_out = out.data();
            strm.avail_out = static_cast<uInt>(CHUNK);
            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&strm);
                return -8;
            }
            size_t have = CHUNK - strm.avail_out;
            decompressed.insert(decompressed.end(), out.data(), out.data() + have);
        } while (ret != Z_STREAM_END);

        inflateEnd(&strm);

        if (decompressed.size() != original.size()) return -9;
        if (std::memcmp(decompressed.data(), original.data(), original.size()) != 0) return -10;
    }

    // 3) gz* API: write gz file, read it back
    const char* tmpname = "test_zlib_api_sequence_temp.gz";
    {
        gzFile gz = gzopen(tmpname, "wb");
        if (!gz) return -11;
        int written = gzwrite(gz, original.data(), static_cast<unsigned int>(original.size()));
        if (written == 0) { gzclose(gz); return -12; }
        if (gzclose(gz) != Z_OK) return -13;
    }

    {
        gzFile gz = gzopen(tmpname, "rb");
        if (!gz) return -14;
        std::vector<char> readbuf(original.size());
        int read = gzread(gz, readbuf.data(), static_cast<unsigned int>(readbuf.size()));
        if (read < 0) { gzclose(gz); return -15; }
        if (static_cast<size_t>(read) != original.size()) { gzclose(gz); return -16; }
        if (std::memcmp(readbuf.data(), original.data(), original.size()) != 0) { gzclose(gz); return -17; }
        if (gzclose(gz) != Z_OK) return -18;
    }

    // remove temporary file
    std::remove(tmpname);

    // All checks passed
    return 66;
}