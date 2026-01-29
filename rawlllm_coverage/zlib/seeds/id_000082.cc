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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data (make it reasonably compressible)
    const char* input = "The quick brown fox jumps over the lazy dog. "
                        "This is a zlib test sequence. "
                        "Repeat: The quick brown fox jumps over the lazy dog. ";
    size_t input_len = std::strlen(input);
    if (input_len == 0) return -1;

    // 1) Compute checksums (adler32 and crc32)
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));

    // 2) Compress using the streaming deflate API (deflateInit / deflate / deflateEnd)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -2;

    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> compBuf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(bound);

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -3;
    }
    uLong compSize = defstream.total_out;
    deflateEnd(&defstream);

    // 3) Inflate using the streaming inflate API (inflateInit / inflate / inflateEnd)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compSize);

    std::vector<Bytef> decompBuf(input_len);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(input_len);

    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -4;

    // Loop until stream end (robust approach)
    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            inflateEnd(&infstream);
            return -5;
        }
        // If output buffer filled but not finished, that's an error for this test
        if (infstream.avail_out == 0) {
            inflateEnd(&infstream);
            return -6;
        }
        // If input exhausted but not finished, continue (shouldn't happen in this simple test)
        if (infstream.avail_in == 0 && ret != Z_STREAM_END) {
            // attempt one more try with Z_FINISH
            ret = inflate(&infstream, Z_FINISH);
            if (ret != Z_STREAM_END) {
                inflateEnd(&infstream);
                return -7;
            }
            break;
        }
    }

    uLong decompSize = infstream.total_out;
    inflateEnd(&infstream);

    if (decompSize != input_len) return -8;
    if (std::memcmp(decompBuf.data(), input, input_len) != 0) return -9;

    // 4) Use the convenience compress2 / uncompress API as an additional check
    uLong comp2Bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp2Buf(comp2Bound);
    uLong comp2Size = comp2Bound;

    ret = compress2(comp2Buf.data(), &comp2Size,
                    reinterpret_cast<const Bytef*>(input),
                    static_cast<uLong>(input_len),
                    Z_BEST_SPEED);
    if (ret != Z_OK) return -10;

    std::vector<Bytef> uncmpBuf(input_len);
    uLong uncmpSize = static_cast<uLong>(input_len);
    ret = uncompress(uncmpBuf.data(), &uncmpSize, comp2Buf.data(), comp2Size);
    if (ret != Z_OK) return -11;
    if (uncmpSize != input_len) return -12;
    if (std::memcmp(uncmpBuf.data(), input, input_len) != 0) return -13;

    // 5) Use gz* convenience APIs to write/read a .gz file and validate contents
    const char* fname = "test_zlib_api_sequence.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return -14;
    int wrote = gzwrite(gz, input, static_cast<unsigned>(input_len));
    if (wrote <= 0) { gzclose(gz); return -15; }
    gzclose(gz);

    gzFile gzin = gzopen(fname, "rb");
    if (!gzin) return -16;
    std::vector<char> gzReadBuf(input_len + 1);
    int gread = gzread(gzin, gzReadBuf.data(), static_cast<unsigned>(input_len));
    gzclose(gzin);
    if (gread != static_cast<int>(input_len)) return -17;
    if (std::memcmp(gzReadBuf.data(), input, input_len) != 0) return -18;

    // If we reach here, all API sequences succeeded
    (void)adler; // use variables to avoid unused-warning if compiled with strict flags
    (void)crc;

    return 66;
}