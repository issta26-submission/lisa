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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Uses several zlib APIs: adler32, crc32, deflateInit/deflate/deflateEnd,
// inflateInit/inflate/inflateEnd, compress2, uncompress, compressBound.
int test_zlib_api_sequence() {
    // Prepare input data (repeat a phrase to get a reasonably sized buffer)
    const char* phrase = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    for (int i = 0; i < 200; ++i) input += phrase; // ~9000 bytes

    const size_t input_len = input.size();
    const Bytef* input_data = reinterpret_cast<const Bytef*>(input.data());

    // 1) Compute checksums using zlib utilities
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, input_data, static_cast<uInt>(input_len));

    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, input_data, static_cast<uInt>(input_len));

    // 2) Compress using deflate* sequence (stream API)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        deflateEnd(&defstream);
        return -1;
    }

    // Allocate output buffer with a safe upper bound
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp_buf(bound);
    defstream.next_in = const_cast<Bytef*>(input_data);
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = comp_buf.data();
    defstream.avail_out = static_cast<uInt>(comp_buf.size());

    // Perform compression (single-pass should be enough here)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }
    size_t comp_size = comp_buf.size() - defstream.avail_out;
    deflateEnd(&defstream);

    // 3) Decompress using inflate* sequence (stream API)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        inflateEnd(&infstream);
        return -3;
    }

    std::vector<Bytef> decomp_buf(input_len); // we know expected size
    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(decomp_buf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -4;
    }
    size_t decomp_size = decomp_buf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    // Check decompressed data matches the original
    if (decomp_size != input_len) return -5;
    if (std::memcmp(decomp_buf.data(), input.data(), input_len) != 0) return -6;

    // 4) Demonstrate compress2 / uncompress convenience APIs
    uLong comp2_bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp2_buf(comp2_bound);
    uLong comp2_len = comp2_bound;

    ret = compress2(comp2_buf.data(), &comp2_len, input_data, static_cast<uLong>(input_len), Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -7;

    std::vector<Bytef> uncompressed_buf(input_len);
    uLong uncompressed_len = static_cast<uLong>(input_len);
    ret = uncompress(uncompressed_buf.data(), &uncompressed_len, comp2_buf.data(), comp2_len);
    if (ret != Z_OK) return -8;

    if (uncompressed_len != input_len) return -9;
    if (std::memcmp(uncompressed_buf.data(), input.data(), input_len) != 0) return -10;

    // 5) Recompute checksums on decompressed data and compare with original computed values
    uLong adler_check = adler32(0L, Z_NULL, 0);
    adler_check = adler32(adler_check, reinterpret_cast<const Bytef*>(decomp_buf.data()), static_cast<uInt>(decomp_size));
    if (adler_check != adler) return -11;

    uLong crc_check = crc32(0L, Z_NULL, 0);
    crc_check = crc32(crc_check, reinterpret_cast<const Bytef*>(decomp_buf.data()), static_cast<uInt>(decomp_size));
    if (crc_check != crc) return -12;

    // If everything succeeded return the requested success code
    return 66;
}