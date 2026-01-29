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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input
    const char* src = "The quick brown fox jumps over the lazy dog";
    uLong src_len = static_cast<uLong>(std::strlen(src));

    // 1) Simple one-shot compression / decompression using compress() / uncompress()
    uLongf bound = compressBound(src_len);
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_len = bound;
    if (compress(comp_buf.data(), &comp_len, reinterpret_cast<const Bytef*>(src), src_len) != Z_OK) {
        return -1;
    }

    std::vector<Bytef> decomp_buf(src_len);
    uLongf decomp_len = src_len;
    if (uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len) != Z_OK) {
        return -2;
    }
    if (decomp_len != src_len || std::memcmp(decomp_buf.data(), src, src_len) != 0) {
        return -3;
    }

    // 2) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(src), src_len);
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(src), src_len);

    // 3) Stream compression using deflate()
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    if (deflateInit(&dstrm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -4;
    }
    // set input
    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    dstrm.avail_in = static_cast<uInt>(src_len);
    // set output buffer (use compressBound to be safe)
    std::vector<Bytef> streamed_comp(bound);
    dstrm.next_out = streamed_comp.data();
    dstrm.avail_out = static_cast<uInt>(bound);

    int ret = deflate(&dstrm, Z_FINISH);
    // Because we sized output with compressBound, typically we get Z_STREAM_END immediately.
    while (ret == Z_OK) {
        // In the unlikely event we need more output, we'd handle resizing here.
        // For this test function (small input), this branch should not be hit.
        ret = deflate(&dstrm, Z_FINISH);
    }
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        return -5;
    }
    uLong streamed_comp_size = static_cast<uLong>(bound - dstrm.avail_out);
    deflateEnd(&dstrm);

    // 4) Stream decompression using inflate()
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    if (inflateInit(&istrm) != Z_OK) {
        return -6;
    }
    istrm.next_in = streamed_comp.data();
    istrm.avail_in = static_cast<uInt>(streamed_comp_size);
    std::vector<Bytef> streamed_decomp(src_len);
    istrm.next_out = streamed_decomp.data();
    istrm.avail_out = static_cast<uInt>(src_len);

    ret = inflate(&istrm, Z_FINISH);
    while (ret == Z_OK) {
        ret = inflate(&istrm, Z_FINISH);
    }
    if (ret != Z_STREAM_END) {
        inflateEnd(&istrm);
        return -7;
    }
    uLong streamed_decomp_size = static_cast<uLong>(src_len - istrm.avail_out);
    inflateEnd(&istrm);

    if (streamed_decomp_size != src_len || std::memcmp(streamed_decomp.data(), src, src_len) != 0) {
        return -8;
    }

    // 5) Query zlib version and compile flags (demonstrates additional APIs)
    const char* version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    (void)version;        // avoid unused-variable warnings
    (void)compile_flags;
    (void)crc;
    (void)adl;

    // All tests passed for this API usage sequence
    return 66;
}