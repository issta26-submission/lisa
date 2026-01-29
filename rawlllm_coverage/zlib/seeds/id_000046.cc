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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary headers
    #include <zlib.h>
    #include <cstring>
    #include <vector>

    // Sample input data
    const char* sample = "The quick brown fox jumps over the lazy dog. "
                         "Repeat: The quick brown fox jumps over the lazy dog. "
                         "End of sample.";
    size_t sample_len = std::strlen(sample);
    if (sample_len == 0) return 1;

    // 1) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));

    // 2) Use compress2 / uncompress (one-shot API)
    uLong bound = compressBound(static_cast<uLong>(sample_len));
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_size = bound;
    int ret = compress2(comp_buf.data(), &comp_size,
                        reinterpret_cast<const Bytef*>(sample),
                        static_cast<uLong>(sample_len),
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 2;

    // decompress
    std::vector<Bytef> decomp_buf(sample_len);
    uLongf decomp_size = static_cast<uLongf>(sample_len);
    ret = uncompress(decomp_buf.data(), &decomp_size, comp_buf.data(), comp_size);
    if (ret != Z_OK) return 3;
    if (decomp_size != sample_len) return 4;
    if (std::memcmp(decomp_buf.data(), sample, sample_len) != 0) return 5;

    // 3) Stream deflate -> produce a gzip-format stream (deflateInit2 with +16)
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    dstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(sample));
    dstream.avail_in = static_cast<uInt>(sample_len);

    ret = deflateInit2(&dstream,
                       Z_DEFAULT_COMPRESSION,
                       Z_DEFLATED,
                       MAX_WBITS + 16, // +16 to write gzip header
                       8,
                       Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) return 6;

    // Prepare output buffer for gzip data
    std::vector<Bytef> gzip_buf(compressBound(static_cast<uLong>(sample_len)));
    dstream.next_out = gzip_buf.data();
    dstream.avail_out = static_cast<uInt>(gzip_buf.size());

    // Run deflate until finished
    for (;;) {
        ret = deflate(&dstream, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK) {
            // Not finished; but if avail_out==0 we need to grow the buffer (unlikely given compressBound)
            if (dstream.avail_out == 0) {
                size_t old_size = gzip_buf.size();
                gzip_buf.resize(old_size * 2);
                dstream.next_out = gzip_buf.data() + old_size;
                dstream.avail_out = static_cast<uInt>(gzip_buf.size() - old_size);
                continue;
            }
            // Otherwise keep looping
            continue;
        }
        // Unexpected error
        deflateEnd(&dstream);
        return 7;
    }

    // Compute actual gzip size written
    size_t gzip_size = gzip_buf.size() - dstream.avail_out;
    ret = deflateEnd(&dstream);
    if (ret != Z_OK) return 8;

    // 4) Stream inflate (gzip-aware) to recover original data
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    istream.next_in = gzip_buf.data();
    istream.avail_in = static_cast<uInt>(gzip_size);

    ret = inflateInit2(&istream, MAX_WBITS + 16); // +16 to decode gzip
    if (ret != Z_OK) return 9;

    std::vector<Bytef> out_buf(sample_len);
    istream.next_out = out_buf.data();
    istream.avail_out = static_cast<uInt>(out_buf.size());

    for (;;) {
        ret = inflate(&istream, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK) {
            if (istream.avail_out == 0) {
                // Need more room (shouldn't happen for this example), expand buffer
                size_t old = out_buf.size();
                out_buf.resize(old * 2);
                istream.next_out = out_buf.data() + old;
                istream.avail_out = static_cast<uInt>(out_buf.size() - old);
                continue;
            }
            continue;
        }
        inflateEnd(&istream);
        return 10;
    }

    size_t inflated_size = out_buf.size() - istream.avail_out;
    ret = inflateEnd(&istream);
    if (ret != Z_OK) return 11;

    if (inflated_size != sample_len) return 12;
    if (std::memcmp(out_buf.data(), sample, sample_len) != 0) return 13;

    // 5) Verify checksums still match for the recovered data
    uLong crc2 = crc32(0L, Z_NULL, 0);
    crc2 = crc32(crc2, out_buf.data(), static_cast<uInt>(inflated_size));
    if (crc2 != crc) return 14;

    uLong adl2 = adler32(0L, Z_NULL, 0);
    adl2 = adler32(adl2, out_buf.data(), static_cast<uInt>(inflated_size));
    if (adl2 != adl) return 15;

    // If we reached here, all zlib API sequences used above succeeded and produced correct results.
    return 66;
}