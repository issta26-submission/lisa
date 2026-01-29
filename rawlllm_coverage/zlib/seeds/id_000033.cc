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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Example input to compress and decompress
    const char *input = "The quick brown fox jumps over the lazy dog. "
                        "This is a test of zlib compression and decompression APIs.";
    size_t input_len = strlen(input);

    // ---------------------
    // 1) Using deflate/inflate (stream API)
    // ---------------------
    // Prepare deflate stream
    z_stream defstrm;
    defstrm.zalloc = Z_NULL;
    defstrm.zfree = Z_NULL;
    defstrm.opaque = Z_NULL;

    int ret = deflateInit(&defstrm, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 1;

    // Allocate enough space for compressed data
    uLong bound = compressBound((uLong)input_len);
    std::vector<Bytef> comp_buf((size_t)bound);

    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)input_len;
    defstrm.next_out = comp_buf.data();
    defstrm.avail_out = (uInt)bound;

    // Perform compression in a single call (buffer large enough because of compressBound)
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&defstrm); return 2; }

    size_t comp_size = defstrm.total_out;
    deflateEnd(&defstrm);

    // Prepare inflate stream
    z_stream infstrm;
    infstrm.zalloc = Z_NULL;
    infstrm.zfree = Z_NULL;
    infstrm.opaque = Z_NULL;

    ret = inflateInit(&infstrm);
    if (ret != Z_OK) return 3;

    std::vector<Bytef> decomp_buf(input_len);
    infstrm.next_in = comp_buf.data();
    infstrm.avail_in = (uInt)comp_size;
    infstrm.next_out = decomp_buf.data();
    infstrm.avail_out = (uInt)input_len;

    // Perform decompression
    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&infstrm); return 4; }

    size_t decomp_size = infstrm.total_out;
    inflateEnd(&infstrm);

    if (decomp_size != input_len) return 5;
    if (memcmp(decomp_buf.data(), input, input_len) != 0) return 6;

    // ---------------------
    // 2) Using convenience APIs compress2 / uncompress
    // ---------------------
    uLong bound2 = compressBound((uLong)input_len);
    std::vector<Bytef> comp2((size_t)bound2);
    uLong comp2_size = bound2;

    ret = compress2(comp2.data(), &comp2_size, (const Bytef *)input, (uLong)input_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 7;

    std::vector<Bytef> decomp2(input_len);
    uLong decomp2_size = (uLong)input_len;
    ret = uncompress(decomp2.data(), &decomp2_size, comp2.data(), comp2_size);
    if (ret != Z_OK) return 8;

    if (decomp2_size != input_len) return 9;
    if (memcmp(decomp2.data(), input, input_len) != 0) return 10;

    // ---------------------
    // 3) crc32 check (optional verification API)
    // ---------------------
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (const Bytef *)input, (uInt)input_len);
    (void)crc; // just demonstrate API; not used further

    // If all checks passed, return 66 as requested
    return 66;
}