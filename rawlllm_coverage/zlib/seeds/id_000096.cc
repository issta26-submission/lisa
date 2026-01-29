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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input
    const char input_data[] =
        "The quick brown fox jumps over the lazy dog. "
        "Zlib API usage test string to compress and decompress.";
    uLong input_len = static_cast<uLong>(std::strlen(input_data));

    // 1) Simple compress() / uncompress() pair
    uLong comp_bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(comp_bound);
    uLong comp_len = comp_bound;
    int ret = compress(comp_buf.data(), &comp_len, reinterpret_cast<const Bytef*>(input_data), input_len);
    if (ret != Z_OK) return 1;

    std::vector<Bytef> uncmp_buf(input_len + 1); // +1 to be safe for any string ops
    uLong uncmp_len = input_len;
    ret = uncompress(uncmp_buf.data(), &uncmp_len, comp_buf.data(), comp_len);
    if (ret != Z_OK) return 2;
    if (uncmp_len != input_len) return 3;
    if (std::memcmp(uncmp_buf.data(), input_data, input_len) != 0) return 4;

    // 2) Checksums: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input_data), input_len);
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input_data), input_len);
    (void)crc; // used to demonstrate API; not required further
    (void)adl;

    // 3) Stream API: deflate (stream) then inflate (stream)
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_data));
    dstrm.avail_in = input_len;

    // deflateBound requires an initialized stream pointer
    uLong stream_bound = deflateBound(&dstrm, input_len);
    std::vector<Bytef> stream_comp_buf(stream_bound);
    dstrm.next_out = stream_comp_buf.data();
    dstrm.avail_out = stream_bound;

    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        return 6;
    }
    uLong stream_comp_len = static_cast<uLong>(dstrm.total_out);
    ret = deflateEnd(&dstrm);
    if (ret != Z_OK) return 7;

    // inflate
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    ret = inflateInit(&istrm);
    if (ret != Z_OK) return 8;

    istrm.next_in = stream_comp_buf.data();
    istrm.avail_in = stream_comp_len;
    std::vector<Bytef> stream_uncmp_buf(input_len);
    istrm.next_out = stream_uncmp_buf.data();
    istrm.avail_out = input_len;

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istrm);
        return 9;
    }
    uLong stream_uncmp_len = static_cast<uLong>(istrm.total_out);
    ret = inflateEnd(&istrm);
    if (ret != Z_OK) return 10;

    if (stream_uncmp_len != input_len) return 11;
    if (std::memcmp(stream_uncmp_buf.data(), input_data, input_len) != 0) return 12;

    // All checks passed
    return 66;
}