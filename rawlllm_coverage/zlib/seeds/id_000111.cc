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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input (repeat to make compression more meaningful)
    const char *sample = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    for (int i = 0; i < 64; ++i) input += sample; // ~64 repetitions

    const size_t input_len = input.size();

    // 1) One-shot compression using compress2()
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> one_shot_comp(bound);
    uLongf comp_len = bound;
    int ret = compress2(one_shot_comp.data(), &comp_len,
                        reinterpret_cast<const Bytef*>(input.data()),
                        static_cast<uLong>(input_len),
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 1;
    one_shot_comp.resize(comp_len);

    // 2) One-shot decompression using uncompress()
    std::vector<unsigned char> one_shot_decomp(input_len);
    uLongf decomp_len = static_cast<uLongf>(input_len);
    ret = uncompress(one_shot_decomp.data(), &decomp_len,
                     one_shot_comp.data(), static_cast<uLongf>(one_shot_comp.size()));
    if (ret != Z_OK) return 2;
    if (decomp_len != input_len) return 3;
    if (std::memcmp(one_shot_decomp.data(), input.data(), input_len) != 0) return 4;

    // 3) Compute adler32 and crc32 checksums
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_len));

    // Sanity: non-zero checksums for non-empty input
    if (adler == 0 || crc == 0) return 5;

    // 4) Streaming compression using deflateInit / deflate / deflateEnd
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    defstrm.zalloc = Z_NULL;
    defstrm.zfree = Z_NULL;
    defstrm.opaque = Z_NULL;

    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 6;

    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(input_len);

    std::vector<unsigned char> stream_comp;
    const size_t CHUNK = 128;
    unsigned char outbuf[CHUNK];

    // Use Z_FINISH directly when all input is provided; propagate until Z_STREAM_END
    for (;;) {
        defstrm.next_out = outbuf;
        defstrm.avail_out = CHUNK;

        int defret = deflate(&defstrm, Z_FINISH);
        if (defret != Z_OK && defret != Z_STREAM_END) {
            deflateEnd(&defstrm);
            return 7;
        }

        size_t have = CHUNK - defstrm.avail_out;
        if (have) stream_comp.insert(stream_comp.end(), outbuf, outbuf + have);

        if (defret == Z_STREAM_END) break;
        // Otherwise loop until Z_STREAM_END
    }

    deflateEnd(&defstrm);

    if (stream_comp.empty()) return 8;

    // 5) Streaming decompression using inflateInit / inflate / inflateEnd
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    infstrm.zalloc = Z_NULL;
    infstrm.zfree = Z_NULL;
    infstrm.opaque = Z_NULL;

    ret = inflateInit(&infstrm);
    if (ret != Z_OK) return 9;

    infstrm.next_in = stream_comp.data();
    infstrm.avail_in = static_cast<uInt>(stream_comp.size());

    std::vector<unsigned char> stream_decomp;
    unsigned char inf_outbuf[CHUNK];

    for (;;) {
        infstrm.next_out = inf_outbuf;
        infstrm.avail_out = CHUNK;

        int infret = inflate(&infstrm, Z_NO_FLUSH);
        if (infret != Z_OK && infret != Z_STREAM_END && infret != Z_BUF_ERROR) {
            inflateEnd(&infstrm);
            return 10;
        }

        size_t got = CHUNK - infstrm.avail_out;
        if (got) stream_decomp.insert(stream_decomp.end(), inf_outbuf, inf_outbuf + got);

        if (infret == Z_STREAM_END) break;
        // If Z_BUF_ERROR and no output was produced, break as well to avoid infinite loop
        if (infret == Z_BUF_ERROR && infstrm.avail_in == 0) break;
    }

    inflateEnd(&infstrm);

    // 6) Validate streaming decompressed data equals original
    if (stream_decomp.size() != input_len) return 11;
    if (std::memcmp(stream_decomp.data(), input.data(), input_len) != 0) return 12;

    // If we reach here, all tests succeeded
    (void)adler; // avoid unused-variable warnings in some environments
    (void)crc;

    return 66;
}