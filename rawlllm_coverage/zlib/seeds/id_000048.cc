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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Test a sequence of common zlib APIs: crc32/adler32, deflate/inflate (streaming),
// compress/uncompress, and compress2. Returns 66 on success, other values indicate failure.
int test_zlib_api_sequence() {
    const char* input_cstr = "The quick brown fox jumps over the lazy dog";
    const size_t input_len = std::strlen(input_cstr);

    // 1) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input_cstr), static_cast<uInt>(input_len));

    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input_cstr), static_cast<uInt>(input_len));

    // 2) Use the deflate (stream) API to compress data
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    if (deflateInit(&defstream, Z_BEST_COMPRESSION) != Z_OK) {
        return -1;
    }

    // allocate output buffer using compressBound to be safe
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> compressed(bound);
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_cstr));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = compressed.data();
    defstream.avail_out = static_cast<uInt>(compressed.size());

    int def_ret = deflate(&defstream, Z_FINISH);
    if (def_ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }
    uLong compressed_size = static_cast<uLong>(compressed.size() - defstream.avail_out);

    if (deflateEnd(&defstream) != Z_OK) {
        return -3;
    }

    // 3) Use the inflate (stream) API to decompress and verify equality
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    if (inflateInit(&infstream) != Z_OK) {
        return -4;
    }

    std::vector<unsigned char> decompressed(input_len); // we know expected size
    infstream.next_in = compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);
    infstream.next_out = decompressed.data();
    infstream.avail_out = static_cast<uInt>(decompressed.size());

    int inf_ret = inflate(&infstream, Z_FINISH);
    if (inf_ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5;
    }
    uLong decompressed_size = static_cast<uLong>(decompressed.size() - infstream.avail_out);

    if (inflateEnd(&infstream) != Z_OK) {
        return -6;
    }

    // Verify sizes and contents match original
    if (decompressed_size != input_len) {
        return -7;
    }
    if (std::memcmp(decompressed.data(), input_cstr, input_len) != 0) {
        return -8;
    }

    // 4) Test compress() and uncompress() simple API
    uLongf dest_len = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> comp_simple(dest_len);
    int cmp_ret = compress(comp_simple.data(), &dest_len,
                           reinterpret_cast<const Bytef*>(input_cstr),
                           static_cast<uLong>(input_len));
    if (cmp_ret != Z_OK) {
        return -9;
    }

    // Uncompress back
    uLongf uncomp_len = static_cast<uLongf>(input_len);
    std::vector<unsigned char> uncomp_simple(uncomp_len);
    int unc_ret = uncompress(uncomp_simple.data(), &uncomp_len,
                             comp_simple.data(), dest_len);
    if (unc_ret != Z_OK) {
        return -10;
    }
    if (uncomp_len != input_len) {
        return -11;
    }
    if (std::memcmp(uncomp_simple.data(), input_cstr, input_len) != 0) {
        return -12;
    }

    // 5) Test compress2() with an explicit level
    uLongf comp2_dest_len = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> comp2_buf(comp2_dest_len);
    int cmp2_ret = compress2(comp2_buf.data(), &comp2_dest_len,
                             reinterpret_cast<const Bytef*>(input_cstr),
                             static_cast<uLong>(input_len),
                             Z_BEST_SPEED);
    if (cmp2_ret != Z_OK) {
        return -13;
    }

    // uncompress the compress2 result
    uLongf uncomp2_len = static_cast<uLongf>(input_len);
    std::vector<unsigned char> uncomp2_buf(uncomp2_len);
    int unc2_ret = uncompress(uncomp2_buf.data(), &uncomp2_len,
                              comp2_buf.data(), comp2_dest_len);
    if (unc2_ret != Z_OK) {
        return -14;
    }
    if (uncomp2_len != input_len) {
        return -15;
    }
    if (std::memcmp(uncomp2_buf.data(), input_cstr, input_len) != 0) {
        return -16;
    }

    // All tests passed -> return 66 as requested
    (void)crc;   // silence unused-variable warnings when building in some contexts
    (void)adler; // we computed them as part of the sequence

    return 66;
}