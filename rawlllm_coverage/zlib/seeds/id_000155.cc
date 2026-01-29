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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Local includes so the function is self-contained when pasted alone.
    // (Preprocessor directives are allowed anywhere before compilation.)
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>

    // A short test payload.
    const char* original = "The quick brown fox jumps over the lazy dog. zlib test payload.";
    const size_t original_len = std::strlen(original);

    // 1) Use compressBound + compress2
    uLong bound = compressBound(static_cast<uLong>(original_len));
    std::vector<unsigned char> comp_buf(bound);
    uLongf comp_size = static_cast<uLongf>(bound);
    int ret = compress2(comp_buf.data(), &comp_size,
                        reinterpret_cast<const Bytef*>(original),
                        static_cast<uLong>(original_len),
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 1; // error

    // 2) Use crc32 and adler32 on original data
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(original_len));
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(original_len));

    // 3) Use uncompress to verify we can restore the original
    std::vector<unsigned char> uncomp_buf(original_len + 1); // +1 for safety / null
    uLongf uncompr_len = static_cast<uLongf>(original_len);
    ret = uncompress(uncomp_buf.data(), &uncompr_len,
                     comp_buf.data(), comp_size);
    if (ret != Z_OK) return 2;
    // Compare
    if (uncompr_len != original_len) return 3;
    if (std::memcmp(uncomp_buf.data(), original, original_len) != 0) return 4;

    // 4) Streaming deflate (deflateInit / deflate / deflateEnd)
    {
        z_stream strm;
        std::memset(&strm, 0, sizeof(strm));
        ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return 5;

        // provide input in one chunk
        strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
        strm.avail_in = static_cast<uInt>(original_len);

        std::vector<unsigned char> out_stream;
        out_stream.reserve(128);

        unsigned char out_chunk[64];
        int flush = Z_FINISH;
        do {
            strm.next_out = out_chunk;
            strm.avail_out = sizeof(out_chunk);
            ret = deflate(&strm, flush);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&strm);
                return 6;
            }
            size_t written = sizeof(out_chunk) - strm.avail_out;
            out_stream.insert(out_stream.end(), out_chunk, out_chunk + written);
        } while (ret != Z_STREAM_END);
        deflateEnd(&strm);

        // Now inflate the stream we just created to verify streaming paths
        z_stream istrm;
        std::memset(&istrm, 0, sizeof(istrm));
        ret = inflateInit(&istrm);
        if (ret != Z_OK) return 7;

        istrm.next_in = out_stream.empty() ? Z_NULL : out_stream.data();
        istrm.avail_in = static_cast<uInt>(out_stream.size());

        std::vector<unsigned char> restored;
        restored.reserve(original_len + 8);
        unsigned char in_chunk[32];
        do {
            istrm.next_out = in_chunk;
            istrm.avail_out = sizeof(in_chunk);
            ret = inflate(&istrm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&istrm);
                return 8;
            }
            size_t got = sizeof(in_chunk) - istrm.avail_out;
            restored.insert(restored.end(), in_chunk, in_chunk + got);
        } while (ret != Z_STREAM_END);
        inflateEnd(&istrm);

        if (restored.size() != original_len) return 9;
        if (std::memcmp(restored.data(), original, original_len) != 0) return 10;
    }

    // 5) Use deflateInit2/inflateInit2 to create and read gzip-format compressed data (windowBits = 15 + 16)
    {
        z_stream gz_strm;
        std::memset(&gz_strm, 0, sizeof(gz_strm));
        int windowBits = 15 + 16; // gzip header
        ret = deflateInit2(&gz_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           windowBits, 8, Z_DEFAULT_STRATEGY);
        if (ret != Z_OK) return 11;

        gz_strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
        gz_strm.avail_in = static_cast<uInt>(original_len);

        std::vector<unsigned char> gz_out;
        unsigned char obuf[128];
        do {
            gz_strm.next_out = obuf;
            gz_strm.avail_out = sizeof(obuf);
            ret = deflate(&gz_strm, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&gz_strm);
                return 12;
            }
            size_t wrote = sizeof(obuf) - gz_strm.avail_out;
            gz_out.insert(gz_out.end(), obuf, obuf + wrote);
        } while (ret != Z_STREAM_END);
        deflateEnd(&gz_strm);

        // Inflate with inflateInit2 using same windowBits
        z_stream gz_istrm;
        std::memset(&gz_istrm, 0, sizeof(gz_istrm));
        ret = inflateInit2(&gz_istrm, windowBits);
        if (ret != Z_OK) return 13;

        gz_istrm.next_in = gz_out.empty() ? Z_NULL : gz_out.data();
        gz_istrm.avail_in = static_cast<uInt>(gz_out.size());

        std::vector<unsigned char> gz_restored;
        unsigned char ibuf[64];
        do {
            gz_istrm.next_out = ibuf;
            gz_istrm.avail_out = sizeof(ibuf);
            ret = inflate(&gz_istrm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&gz_istrm);
                return 14;
            }
            size_t got = sizeof(ibuf) - gz_istrm.avail_out;
            gz_restored.insert(gz_restored.end(), ibuf, ibuf + got);
        } while (ret != Z_STREAM_END);
        inflateEnd(&gz_istrm);

        if (gz_restored.size() != original_len) return 15;
        if (std::memcmp(gz_restored.data(), original, original_len) != 0) return 16;
    }

    // If we reached this point, all zlib API sequences used above worked as expected.
    // Return the required success code.
    return 66;
}