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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Put includes here so the function is self-contained when pasted into a file.
    // Including inside a function is legal in C/C++, and keeps this snippet standalone.
    #include <zlib.h>
    #include <vector>
    #include <cstring> // for memcmp, memset

    // Prepare test input: a repeating pattern to ensure compressibility.
    const size_t src_len = 1024;
    std::vector<Bytef> src(src_len);
    for (size_t i = 0; i < src_len; ++i) src[i] = static_cast<Bytef>('A' + (i % 26));

    // 1) Query zlib version (sanity call)
    const char *ver = zlibVersion();
    (void)ver; // silence unused variable warning

    // 2) Basic compress() / uncompress() sequence (simple one-shot API)
    uLongf comp_bound = compressBound(static_cast<uLong>(src_len));
    std::vector<Bytef> comp1(comp_bound);
    uLongf comp1_len = comp_bound;
    int ret = compress(comp1.data(), &comp1_len, src.data(), static_cast<uLong>(src_len));
    if (ret != Z_OK) return -1;

    std::vector<Bytef> uncomp1(src_len);
    uLongf uncomp1_len = static_cast<uLong>(src_len);
    ret = uncompress(uncomp1.data(), &uncomp1_len, comp1.data(), comp1_len);
    if (ret != Z_OK) return -2;
    if (uncomp1_len != src_len) return -3;
    if (std::memcmp(src.data(), uncomp1.data(), src_len) != 0) return -4;

    // 3) compress2() with explicit compression level
    std::vector<Bytef> comp2(comp_bound);
    uLongf comp2_len = comp_bound;
    ret = compress2(comp2.data(), &comp2_len, src.data(), static_cast<uLong>(src_len), Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -5;

    // Verify uncompress of compress2 output
    std::vector<Bytef> uncomp2(src_len);
    uLongf uncomp2_len = static_cast<uLong>(src_len);
    ret = uncompress(uncomp2.data(), &uncomp2_len, comp2.data(), comp2_len);
    if (ret != Z_OK) return -6;
    if (uncomp2_len != src_len) return -7;
    if (std::memcmp(src.data(), uncomp2.data(), src_len) != 0) return -8;

    // 4) Compute checksums: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, src.data(), static_cast<uInt>(src_len));
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, src.data(), static_cast<uInt>(src_len));
    (void)crc;
    (void)adler;

    // 5) Streaming API: deflateInit / deflate / deflateEnd
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    ret = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -9;

    // Use compressBound to allocate a safe output buffer for streaming deflate
    std::vector<Bytef> stream_comp(compressBound(static_cast<uLong>(src_len)));
    dstream.next_in = src.data();
    dstream.avail_in = static_cast<uInt>(src_len);
    dstream.next_out = stream_comp.data();
    dstream.avail_out = static_cast<uInt>(stream_comp.size());

    ret = deflate(&dstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstream);
        return -10;
    }
    uLongf stream_comp_len = dstream.total_out;
    deflateEnd(&dstream);

    // 6) Streaming API: inflateInit / inflate / inflateEnd
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    ret = inflateInit(&istream);
    if (ret != Z_OK) return -11;

    std::vector<Bytef> stream_uncomp(src_len);
    istream.next_in = stream_comp.data();
    istream.avail_in = static_cast<uInt>(stream_comp_len);
    istream.next_out = stream_uncomp.data();
    istream.avail_out = static_cast<uInt>(stream_uncomp.size());

    ret = inflate(&istream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istream);
        return -12;
    }
    uLongf stream_uncomp_len = istream.total_out;
    inflateEnd(&istream);

    if (stream_uncomp_len != src_len) return -13;
    if (std::memcmp(src.data(), stream_uncomp.data(), src_len) != 0) return -14;

    // 7) Demonstrate deflateBound usage (obtain safe upper bound for compressed size)
    // deflateBound can be called with a (possibly null) z_stream pointer in modern zlib.
    uLong def_bound = deflateBound(nullptr, static_cast<uLong>(src_len));
    if (def_bound < 1) return -15;
    (void)def_bound;

    // All checks passed; return the required success value.
    return 66;
}