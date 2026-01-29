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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function exercises multiple zlib APIs:
    // - crc32, adler32
    // - compress2 / uncompress
    // - deflateInit/deflate/deflateEnd (streaming deflate)
    // - inflateInit/inflate/inflateEnd (streaming inflate)
    //
    // It returns 66 on success, other values on failure.

    const char* src = "The quick brown fox jumps over the lazy dog";
    const size_t src_len = strlen(src);

    // 1) Compute checksums on original data
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before, reinterpret_cast<const Bytef*>(src), static_cast<uInt>(src_len));
    uLong adler_before = adler32(0L, Z_NULL, 0);
    adler_before = adler32(adler_before, reinterpret_cast<const Bytef*>(src), static_cast<uInt>(src_len));

    // 2) Simple one-shot compression (compress2) and decompression (uncompress)
    uLong bound = compressBound(static_cast<uLong>(src_len));
    Bytef* comp_buf = new (std::nothrow) Bytef[bound];
    if (!comp_buf) return 1;
    uLong comp_len = bound;
    int ret = compress2(comp_buf, &comp_len, reinterpret_cast<const Bytef*>(src), static_cast<uLong>(src_len), Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        delete [] comp_buf;
        return 2;
    }

    // Decompress into buffer of original size
    Bytef* decomp_buf = new (std::nothrow) Bytef[src_len];
    if (!decomp_buf) {
        delete [] comp_buf;
        return 3;
    }
    uLong decomp_len = static_cast<uLong>(src_len);
    ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    if (ret != Z_OK || decomp_len != src_len) {
        delete [] comp_buf;
        delete [] decomp_buf;
        return 4;
    }

    // Validate data equality and checksums
    if (memcmp(decomp_buf, src, src_len) != 0) {
        delete [] comp_buf;
        delete [] decomp_buf;
        return 5;
    }
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, decomp_buf, static_cast<uInt>(decomp_len));
    if (crc_after != crc_before) {
        delete [] comp_buf;
        delete [] decomp_buf;
        return 6;
    }
    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, decomp_buf, static_cast<uInt>(decomp_len));
    if (adler_after != adler_before) {
        delete [] comp_buf;
        delete [] decomp_buf;
        return 7;
    }

    // 3) Streaming deflate
    z_stream defstream;
    memset(&defstream, 0, sizeof(defstream));
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        delete [] comp_buf;
        delete [] decomp_buf;
        return 8;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstream.avail_in = static_cast<uInt>(src_len);

    // Provide an output buffer larger than compressBound for safety
    uLong stream_out_size = compressBound(static_cast<uLong>(src_len));
    Bytef* stream_out = new (std::nothrow) Bytef[stream_out_size];
    if (!stream_out) {
        deflateEnd(&defstream);
        delete [] comp_buf;
        delete [] decomp_buf;
        return 9;
    }
    defstream.next_out = stream_out;
    defstream.avail_out = static_cast<uInt>(stream_out_size);

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        delete [] stream_out;
        delete [] comp_buf;
        delete [] decomp_buf;
        return 10;
    }
    uLong stream_comp_len = stream_out_size - defstream.avail_out;

    if (deflateEnd(&defstream) != Z_OK) {
        delete [] stream_out;
        delete [] comp_buf;
        delete [] decomp_buf;
        return 11;
    }

    // 4) Streaming inflate
    z_stream infstream;
    memset(&infstream, 0, sizeof(infstream));
    if (inflateInit(&infstream) != Z_OK) {
        delete [] stream_out;
        delete [] comp_buf;
        delete [] decomp_buf;
        return 12;
    }

    infstream.next_in = stream_out;
    infstream.avail_in = static_cast<uInt>(stream_comp_len);

    Bytef* stream_decomp = new (std::nothrow) Bytef[src_len];
    if (!stream_decomp) {
        inflateEnd(&infstream);
        delete [] stream_out;
        delete [] comp_buf;
        delete [] decomp_buf;
        return 13;
    }
    infstream.next_out = stream_decomp;
    infstream.avail_out = static_cast<uInt>(src_len);

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END || (src_len - infstream.avail_out) != src_len) {
        inflateEnd(&infstream);
        delete [] stream_decomp;
        delete [] stream_out;
        delete [] comp_buf;
        delete [] decomp_buf;
        return 14;
    }

    if (inflateEnd(&infstream) != Z_OK) {
        delete [] stream_decomp;
        delete [] stream_out;
        delete [] comp_buf;
        delete [] decomp_buf;
        return 15;
    }

    // Verify streaming-decompressed data == original
    if (memcmp(stream_decomp, src, src_len) != 0) {
        delete [] stream_decomp;
        delete [] stream_out;
        delete [] comp_buf;
        delete [] decomp_buf;
        return 16;
    }

    // Cleanup
    delete [] stream_decomp;
    delete [] stream_out;
    delete [] comp_buf;
    delete [] decomp_buf;

    // If we reach here, all tested API sequences succeeded
    return 66;
}