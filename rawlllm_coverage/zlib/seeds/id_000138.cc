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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input data
    const std::string input_str = "The quick brown fox jumps over the lazy dog. "
                                  "This is a short test string for zlib API usage.";

    // Copy input into a mutable buffer for zlib stream-based APIs
    std::vector<unsigned char> src(input_str.begin(), input_str.end());
    uLong src_len = static_cast<uLong>(src.size());

    // 1) High-level compress/uncompress using compress() and uncompress()
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(bound);
    uLongf comp_len = static_cast<uLongf>(bound);

    int ret = compress(comp_buf.data(), &comp_len, src.data(), src_len);
    if (ret != Z_OK) return -1; // compress failed

    // Prepare buffer to uncompress into (we know original size)
    std::vector<unsigned char> uncmp_buf(src_len);
    uLongf uncmp_len = static_cast<uLongf>(src_len);
    ret = uncompress(uncmp_buf.data(), &uncmp_len, comp_buf.data(), comp_len);
    if (ret != Z_OK) return -2; // uncompress failed

    // Verify round-trip equality
    if (uncmp_len != src_len) return -3;
    if (std::memcmp(uncmp_buf.data(), src.data(), src_len) != 0) return -4;

    // 2) Compute checksums: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, src.data(), src_len);
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, src.data(), src_len);
    // (We don't require particular values here, just that the calls succeed.)

    // 3) Stream-based compression with deflateInit/deflate/deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // Provide input and output buffers
    defstream.next_in = src.data();
    defstream.avail_in = static_cast<uInt>(src_len); // safe for small test data
    std::vector<unsigned char> def_out_buf(bound);
    defstream.next_out = def_out_buf.data();
    defstream.avail_out = static_cast<uInt>(bound);

    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) return -5;
    int def_ret = deflate(&defstream, Z_FINISH);
    if (def_ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -6; // deflate did not finish as expected
    }
    uLong deflated_size = defstream.total_out;
    if (deflateEnd(&defstream) != Z_OK) return -7;

    // 4) Stream-based decompression with inflateInit/inflate/inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.next_in = def_out_buf.data();
    infstream.avail_in = static_cast<uInt>(deflated_size);
    std::vector<unsigned char> inf_out_buf(src_len);
    infstream.next_out = inf_out_buf.data();
    infstream.avail_out = static_cast<uInt>(src_len);

    if (inflateInit(&infstream) != Z_OK) return -8;
    int inf_ret = inflate(&infstream, Z_FINISH);
    if (inf_ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -9; // inflate did not finish as expected
    }
    uLong inflated_size = infstream.total_out;
    if (inflateEnd(&infstream) != Z_OK) return -10;

    // Verify that stream-based decompression matches original
    if (inflated_size != src_len) return -11;
    if (std::memcmp(inf_out_buf.data(), src.data(), src_len) != 0) return -12;

    // 5) Query zlib version string (sanity check)
    const char* ver = zlibVersion();
    if (!ver) return -13;

    // If we reached here, all tested API sequences behaved as expected
    return 66;
}