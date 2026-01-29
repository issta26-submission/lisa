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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a sequence of common zlib API usages:
    // - computing checksums (adler32, crc32)
    // - compress/uncompress (one-shot)
    // - deflate/inflate (streaming)
    // It returns 66 on success, other negative values on failure.
    //
    // Note: This function expects that the zlib header is available:
    //     #include <zlib.h>
    // and the C++ standard headers:
    //     #include <string>, <vector>, <cstring>
    //
    // To keep this as a single function (as requested), we assume those
    // headers are present in the translation unit that includes this function.

    // Ensure zlib header is included in the TU; if not, compilation will fail.
    // We'll include the minimal standard headers inside the function body scope,
    // which is allowed for preprocessor directives.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Input data to compress/decompress
    const std::string input = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string for zlib API sequence usage. "
        "Repeat: The quick brown fox jumps over the lazy dog.";

    // Basic checks
    if (input.empty()) return -1;

    // Compute checksums
    uLong input_len = static_cast<uLong>(input.size());
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input.data()), input_len);

    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), input_len);

    // One-shot compress/uncompress using compress2 and uncompress
    uLong bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_size = static_cast<uLongf>(bound);

    int ret = compress2(comp_buf.data(), &comp_size,
                        reinterpret_cast<const Bytef*>(input.data()), input_len,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -2;
    comp_buf.resize(comp_size);

    // Uncompress
    std::vector<Bytef> uncmp_buf(input_len);
    uLongf uncmp_size = static_cast<uLongf>(input_len);
    ret = uncompress(uncmp_buf.data(), &uncmp_size, comp_buf.data(), comp_size);
    if (ret != Z_OK) return -3;
    if (uncmp_size != input_len) return -4;
    if (std::memcmp(uncmp_buf.data(), input.data(), input_len) != 0) return -5;

    // Streaming deflate
    z_stream deflate_strm;
    std::memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;

    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -6;

    // Provide input
    deflate_strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    deflate_strm.avail_in = static_cast<uInt>(input_len); // safe: input small

    // Prepare output buffer
    std::vector<Bytef> scomp_buf(compressBound(input_len));
    deflate_strm.next_out = scomp_buf.data();
    deflate_strm.avail_out = static_cast<uInt>(scomp_buf.size());

    // Finish compression in one call (since data is small)
    ret = deflate(&deflate_strm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&deflate_strm);
        return -7;
    }
    size_t scomp_size = scomp_buf.size() - deflate_strm.avail_out;
    deflateEnd(&deflate_strm);
    scomp_buf.resize(scomp_size);

    // Streaming inflate
    z_stream inflate_strm;
    std::memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;

    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) return -8;

    inflate_strm.next_in = scomp_buf.data();
    inflate_strm.avail_in = static_cast<uInt>(scomp_size);

    std::vector<Bytef> suncmp_buf(input_len);
    inflate_strm.next_out = suncmp_buf.data();
    inflate_strm.avail_out = static_cast<uInt>(suncmp_buf.size());

    ret = inflate(&inflate_strm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&inflate_strm);
        return -9;
    }
    inflateEnd(&inflate_strm);

    if (std::memcmp(suncmp_buf.data(), input.data(), input_len) != 0) return -10;

    // Additional sanity: confirm zlib version string is accessible
    const char* ver = zlibVersion();
    if (ver == nullptr || ver[0] == '\0') return -11;

    // If all checks passed, return 66 as requested.
    (void)adler; // silence unused-variable warnings in release builds
    (void)crc;
    (void)ver;

    return 66;
}