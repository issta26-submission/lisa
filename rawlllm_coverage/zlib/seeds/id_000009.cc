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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input
    const char *input = "The quick brown fox jumps over the lazy dog. "
                        "This is a test string to exercise zlib's API. "
                        "We will compress and decompress it multiple ways.";
    size_t input_len = std::strlen(input);
    if (input_len == 0) return -1;

    // 1) Compute checksums (adler32 and crc32)
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef *>(input), static_cast<uInt>(input_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef *>(input), static_cast<uInt>(input_len));

    // 2) Simple compress / uncompress using the high-level API
    uLong comp_bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp1(comp_bound);
    uLong comp1_len = comp_bound;
    int ret = compress(comp1.data(), &comp1_len,
                       reinterpret_cast<const Bytef *>(input),
                       static_cast<uLong>(input_len));
    if (ret != Z_OK) return -1;

    std::vector<Bytef> decomp1(input_len);
    uLong decomp1_len = static_cast<uLong>(input_len);
    ret = uncompress(decomp1.data(), &decomp1_len, comp1.data(), comp1_len);
    if (ret != Z_OK) return -1;
    if (decomp1_len != input_len) return -1;
    if (std::memcmp(decomp1.data(), input, input_len) != 0) return -1;

    // 3) Use streaming API: deflateInit, deflateBound, deflate, deflateEnd
    z_stream orig;
    std::memset(&orig, 0, sizeof(orig));
    ret = deflateInit(&orig, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -1;

    // deflateBound needs an initialized stream
    uLong bound2 = deflateBound(&orig, static_cast<uLong>(input_len));
    std::vector<Bytef> comp2(bound2);

    // 4) Demonstrate deflateCopy to duplicate a stream state
    z_stream copy;
    std::memset(&copy, 0, sizeof(copy));
    ret = deflateCopy(&copy, &orig);
    if (ret != Z_OK) { deflateEnd(&orig); return -1; }

    // Compress using the original stream
    orig.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input));
    orig.avail_in = static_cast<uInt>(input_len);
    orig.next_out = comp2.data();
    orig.avail_out = static_cast<uInt>(bound2);

    ret = deflate(&orig, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&orig); deflateEnd(&copy); return -1; }
    size_t comp2_len = bound2 - orig.avail_out;
    ret = deflateEnd(&orig);
    if (ret != Z_OK) { deflateEnd(&copy); return -1; }

    // 5) Modify parameters on the copied stream and compress again
    ret = deflateParams(&copy, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) { deflateEnd(&copy); return -1; }

    uLong bound3 = deflateBound(&copy, static_cast<uLong>(input_len));
    std::vector<Bytef> comp3(bound3);
    copy.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input));
    copy.avail_in = static_cast<uInt>(input_len);
    copy.next_out = comp3.data();
    copy.avail_out = static_cast<uInt>(bound3);

    ret = deflate(&copy, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&copy); return -1; }
    size_t comp3_len = bound3 - copy.avail_out;
    ret = deflateEnd(&copy);
    if (ret != Z_OK) return -1;

    // 6) Inflate the first compressed buffer using streaming inflate
    z_stream inf;
    std::memset(&inf, 0, sizeof(inf));
    ret = inflateInit(&inf);
    if (ret != Z_OK) return -1;

    std::vector<Bytef> decomp2(input_len);
    inf.next_in = comp2.data();
    inf.avail_in = static_cast<uInt>(comp2_len);
    inf.next_out = decomp2.data();
    inf.avail_out = static_cast<uInt>(input_len);

    ret = inflate(&inf, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) { inflateEnd(&inf); return -1; }
    size_t decomp2_len = input_len - inf.avail_out;
    ret = inflateEnd(&inf);
    if (ret != Z_OK) return -1;
    if (decomp2_len != input_len) return -1;
    if (std::memcmp(decomp2.data(), input, input_len) != 0) return -1;

    // 7) Inflate the second compressed buffer (compressed with different params)
    z_stream inf2;
    std::memset(&inf2, 0, sizeof(inf2));
    ret = inflateInit(&inf2);
    if (ret != Z_OK) return -1;

    std::vector<Bytef> decomp3(input_len);
    inf2.next_in = comp3.data();
    inf2.avail_in = static_cast<uInt>(comp3_len);
    inf2.next_out = decomp3.data();
    inf2.avail_out = static_cast<uInt>(input_len);

    ret = inflate(&inf2, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) { inflateEnd(&inf2); return -1; }
    size_t decomp3_len = input_len - inf2.avail_out;
    ret = inflateEnd(&inf2);
    if (ret != Z_OK) return -1;
    if (decomp3_len != input_len) return -1;
    if (std::memcmp(decomp3.data(), input, input_len) != 0) return -1;

    // 8) Check zlib version string (sanity check)
    const char *zv = zlibVersion();
    if (zv == nullptr) return -1;
    // (No strict comparison required; we just demonstrated the API.)

    // All operations succeeded
    (void)adler; // used to show checksum calls were made (suppress unused warnings)
    (void)crc;
    (void)zv;
    return 66;
}