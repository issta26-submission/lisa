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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample data to compress
    const char* original = "The quick brown fox jumps over the lazy dog. "
                           "This is a test of zlib APIs including compress2/uncompress, "
                           "deflate/inflate, crc32 and zlibVersion.";
    uLong orig_len = static_cast<uLong>(std::strlen(original));

    // 1) Use compressBound + compress2 to compress in one shot
    uLongf comp_bound = compressBound(orig_len);
    std::vector<Bytef> comp_buf(comp_bound);
    uLongf comp_len = comp_bound;
    int ret = compress2(comp_buf.data(), &comp_len,
                        reinterpret_cast<const Bytef*>(original), orig_len,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return 1; // compress2 failed
    }

    // 2) Use uncompress to get original back
    std::vector<Bytef> decomp_buf(orig_len);
    uLongf decomp_len = orig_len;
    ret = uncompress(decomp_buf.data(), &decomp_len,
                     comp_buf.data(), comp_len);
    if (ret != Z_OK) {
        return 2; // uncompress failed
    }
    if (decomp_len != orig_len) {
        return 3; // unexpected decompressed length
    }
    if (std::memcmp(decomp_buf.data(), original, orig_len) != 0) {
        return 4; // decompressed content mismatch
    }

    // 3) Compute CRC32 of original and of decompressed to demonstrate checksum API
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original), orig_len);
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decomp_buf.data(), decomp_len);
    if (crc_orig != crc_decomp) {
        return 5; // checksum mismatch (should not happen)
    }

    // 4) Demonstrate streaming APIs: deflate (stream) and inflate (stream)
    // deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 6; // deflateInit failed
    }

    // Prepare output buffer larger than needed (use compressBound)
    std::vector<Bytef> stream_comp(comp_bound);
    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(original));
    defstream.avail_in = static_cast<uInt>(orig_len); // safe for our small test data
    defstream.next_out = stream_comp.data();
    defstream.avail_out = static_cast<uInt>(comp_bound);

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 7; // deflate did not finish properly
    }
    uLongf stream_comp_len = defstream.total_out;
    deflateEnd(&defstream);

    // inflate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 8; // inflateInit failed
    }

    std::vector<Bytef> stream_decomp(orig_len);
    infstream.next_in = stream_comp.data();
    infstream.avail_in = static_cast<uInt>(stream_comp_len);
    infstream.next_out = stream_decomp.data();
    infstream.avail_out = static_cast<uInt>(orig_len);

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 9; // inflate did not finish properly
    }
    uLongf stream_decomp_len = infstream.total_out;
    inflateEnd(&infstream);

    if (stream_decomp_len != orig_len) {
        return 10; // streaming decompressed length mismatch
    }
    if (std::memcmp(stream_decomp.data(), original, orig_len) != 0) {
        return 11; // streaming decompressed content mismatch
    }

    // 5) Query zlib version string (non-essential, demonstrates zlibVersion)
    const char* version = zlibVersion();
    if (!version) {
        return 12; // zlibVersion unexpectedly returned null
    }

    // If we got here, all zlib API sequences succeeded
    (void)version; // silence unused variable warnings in build setups where not printed
    return 66;
}