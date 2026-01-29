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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Example input
    const char *input = "The quick brown fox jumps over the lazy dog";
    size_t input_len_sz = strlen(input);
    if (input_len_sz == 0) return 1;
    // zlib uses uLong for sizes in several calls
    uLong input_len = (uLong)input_len_sz;

    // Compute CRC of original data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, (const Bytef *)input, input_len);

    // --- Compress using deflate (z_stream) ---
    z_stream dstream;
    memset(&dstream, 0, sizeof(dstream));
    dstream.zalloc = Z_NULL;
    dstream.zfree = Z_NULL;
    dstream.opaque = Z_NULL;

    int ret = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 2;

    // allocate output buffer large enough
    uLong max_compressed = compressBound(input_len);
    Bytef *comp_buf = new Bytef[max_compressed];
    dstream.next_in = (Bytef *)input;
    dstream.avail_in = input_len;
    dstream.next_out = comp_buf;
    dstream.avail_out = (uInt)max_compressed; // uInt may be smaller, but compressBound fits in uLong; cast for API

    // Run deflate until finished
    ret = deflate(&dstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstream);
        delete[] comp_buf;
        return 3;
    }
    uLong comp_size = dstream.total_out;
    deflateEnd(&dstream);

    // --- Decompress using inflate (z_stream) ---
    z_stream istream;
    memset(&istream, 0, sizeof(istream));
    istream.zalloc = Z_NULL;
    istream.zfree = Z_NULL;
    istream.opaque = Z_NULL;

    ret = inflateInit(&istream);
    if (ret != Z_OK) {
        delete[] comp_buf;
        return 4;
    }

    // allocate output buffer for decompressed data (plus 1 for safety / null-termination)
    uLong decomp_buf_size = input_len + 1;
    Bytef *decomp_buf = new Bytef[decomp_buf_size];
    istream.next_in = comp_buf;
    istream.avail_in = (uInt)comp_size; // safe cast for typical compressed sizes
    istream.next_out = decomp_buf;
    istream.avail_out = (uInt)decomp_buf_size;

    // Inflate until end
    ret = inflate(&istream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istream);
        delete[] comp_buf;
        delete[] decomp_buf;
        return 5;
    }
    uLong decomp_size = istream.total_out;
    inflateEnd(&istream);

    // Null-terminate (safe because we allocated +1)
    if (decomp_size < decomp_buf_size) decomp_buf[decomp_size] = 0;

    // Verify size matches original and content matches
    if (decomp_size != input_len) {
        delete[] comp_buf;
        delete[] decomp_buf;
        return 6;
    }
    if (memcmp(input, decomp_buf, input_len) != 0) {
        delete[] comp_buf;
        delete[] decomp_buf;
        return 7;
    }

    // Verify CRC of decompressed equals original CRC
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, decomp_buf, (uInt)decomp_size);
    delete[] comp_buf;
    delete[] decomp_buf;
    if (crc_after != crc_orig) return 8;

    // All checks passed
    return 66;
}