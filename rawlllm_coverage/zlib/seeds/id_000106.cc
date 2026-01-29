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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Simple input to compress/decompress
    const char* input_cstr = "The quick brown fox jumps over the lazy dog";
    const size_t in_len = std::strlen(input_cstr);
    const Bytef* input = reinterpret_cast<const Bytef*>(input_cstr);

    // 1) Compute checksums on original data
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before, input, static_cast<uInt>(in_len));
    uLong adl_before = adler32(0L, Z_NULL, 0);
    adl_before = adler32(adl_before, input, static_cast<uInt>(in_len));

    // 2) Simple one-shot compression using compress2()
    uLong bound = compressBound(static_cast<uLong>(in_len));
    std::vector<Bytef> compressed(bound);
    uLong compressed_len = bound;
    int ret = compress2(compressed.data(), &compressed_len, input, static_cast<uLong>(in_len), Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1;
    compressed.resize(compressed_len);

    // 3) One-shot decompression using uncompress()
    std::vector<Bytef> decompressed(in_len); // we know original size
    uLong decompressed_len = static_cast<uLong>(in_len);
    ret = uncompress(decompressed.data(), &decompressed_len, compressed.data(), compressed_len);
    if (ret != Z_OK) return -2;
    if (decompressed_len != in_len) return -3;
    if (std::memcmp(decompressed.data(), input, in_len) != 0) return -4;

    // 4) Streaming compression using deflateInit/deflate/deflateEnd
    z_stream defl;
    std::memset(&defl, 0, sizeof(defl));
    ret = deflateInit(&defl, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -10;

    defl.next_in = const_cast<Bytef*>(input);
    defl.avail_in = static_cast<uInt>(in_len);

    // Start with compressBound space; grow if necessary
    std::vector<Bytef> scomp;
    scomp.resize(compressBound(static_cast<uLong>(in_len)));
    defl.next_out = scomp.data();
    defl.avail_out = static_cast<uInt>(scomp.size());

    ret = deflate(&defl, Z_FINISH);
    while (ret == Z_OK) {
        // Need more output space
        size_t used = scomp.size() - defl.avail_out;
        size_t new_size = scomp.size() * 2;
        if (new_size < scomp.size() + 1) new_size = scomp.size() + 1; // avoid zero growth
        scomp.resize(new_size);
        defl.next_out = scomp.data() + used;
        defl.avail_out = static_cast<uInt>(scomp.size() - used);
        ret = deflate(&defl, Z_FINISH);
    }
    if (ret != Z_STREAM_END) {
        deflateEnd(&defl);
        return -11;
    }
    size_t scomp_len = scomp.size() - defl.avail_out;
    deflateEnd(&defl);
    scomp.resize(scomp_len);

    // 5) Streaming decompression using inflateInit/inflate/inflateEnd
    z_stream infl;
    std::memset(&infl, 0, sizeof(infl));
    ret = inflateInit(&infl);
    if (ret != Z_OK) return -20;

    infl.next_in = scomp.data();
    infl.avail_in = static_cast<uInt>(scomp_len);

    // Start with original size; grow if necessary
    std::vector<Bytef> sdecomp;
    sdecomp.resize(in_len);
    infl.next_out = sdecomp.data();
    infl.avail_out = static_cast<uInt>(sdecomp.size());

    ret = inflate(&infl, Z_FINISH);
    while (ret == Z_OK) {
        // Need more output space
        size_t used = sdecomp.size() - infl.avail_out;
        size_t new_size = sdecomp.size() * 2;
        if (new_size < sdecomp.size() + 1) new_size = sdecomp.size() + 1;
        sdecomp.resize(new_size);
        infl.next_out = sdecomp.data() + used;
        infl.avail_out = static_cast<uInt>(sdecomp.size() - used);
        ret = inflate(&infl, Z_FINISH);
    }
    if (ret != Z_STREAM_END) {
        inflateEnd(&infl);
        return -21;
    }
    size_t sdecomp_len = sdecomp.size() - infl.avail_out;
    inflateEnd(&infl);
    sdecomp.resize(sdecomp_len);

    if (sdecomp_len != in_len) return -22;
    if (std::memcmp(sdecomp.data(), input, in_len) != 0) return -23;

    // 6) Compute checksums on decompressed streaming output and compare
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, sdecomp.data(), static_cast<uInt>(sdecomp_len));
    uLong adl_after = adler32(0L, Z_NULL, 0);
    adl_after = adler32(adl_after, sdecomp.data(), static_cast<uInt>(sdecomp_len));

    if (crc_before != crc_after) return -30;
    if (adl_before != adl_after) return -31;

    // All zlib API usage succeeded and integrity checks passed
    return 66;
}