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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function exercises a sequence of zlib APIs:
    // - compress2 / uncompress (one-shot)
    // - deflateInit / deflate / deflateEnd (streaming compress)
    // - inflateInit / inflate / inflateEnd (streaming decompress)
    // - crc32 to validate integrity
    //
    // Requires zlib header/includes in the translation unit where this function is compiled:
    // #include <zlib.h>
    //
    // Returns 66 on success, non-66 on failure.

    const char *sample = "The quick brown fox jumps over the lazy dog. "
                         "Repeat: The quick brown fox jumps over the lazy dog. "
                         "Repeat again: The quick brown fox jumps over the lazy dog.\n";
    unsigned long input_len = (unsigned long)strlen(sample);

    // --- 1) One-shot compression using compress2 ---
    uLongf bound = compressBound(input_len);
    Bytef *cbuf1 = (Bytef *)malloc(bound);
    if (!cbuf1) return -2;
    uLongf csize1 = bound;
    int ret = compress2(cbuf1, &csize1, (const Bytef *)sample, input_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        free(cbuf1);
        return -3;
    }

    // --- 2) One-shot decompression using uncompress ---
    Bytef *out1 = (Bytef *)malloc(input_len + 1);
    if (!out1) { free(cbuf1); return -4; }
    uLongf out1_len = input_len;
    ret = uncompress(out1, &out1_len, cbuf1, csize1);
    if (ret != Z_OK || out1_len != input_len || memcmp(out1, sample, input_len) != 0) {
        free(cbuf1);
        free(out1);
        return -5;
    }
    out1[input_len] = '\0'; // null-terminate (for safety if needed)

    // Compute CRCs and compare
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, (const Bytef *)sample, input_len);
    uLong crc_uncomp = crc32(0L, Z_NULL, 0);
    crc_uncomp = crc32(crc_uncomp, out1, out1_len);
    if (crc_orig != crc_uncomp) {
        free(cbuf1);
        free(out1);
        return -6;
    }

    // --- 3) Streaming compression using deflate ---
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        free(cbuf1);
        free(out1);
        return -7;
    }
    // prepare streaming input
    dstrm.next_in = (Bytef *)sample;
    dstrm.avail_in = input_len;

    // output buffer sized by compressBound
    uLongf bound2 = compressBound(input_len);
    Bytef *cbuf2 = (Bytef *)malloc(bound2);
    if (!cbuf2) {
        deflateEnd(&dstrm);
        free(cbuf1);
        free(out1);
        return -8;
    }
    dstrm.next_out = cbuf2;
    dstrm.avail_out = bound2;

    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        free(cbuf1);
        free(cbuf2);
        free(out1);
        return -9;
    }
    uLongf csize2 = dstrm.total_out;
    deflateEnd(&dstrm);

    // --- 4) Streaming decompression using inflate ---
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    ret = inflateInit(&istrm);
    if (ret != Z_OK) {
        free(cbuf1);
        free(cbuf2);
        free(out1);
        return -10;
    }
    istrm.next_in = cbuf2;
    istrm.avail_in = csize2;

    Bytef *out2 = (Bytef *)malloc(input_len + 1);
    if (!out2) {
        inflateEnd(&istrm);
        free(cbuf1);
        free(cbuf2);
        free(out1);
        return -11;
    }
    istrm.next_out = out2;
    istrm.avail_out = input_len;

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END || istrm.total_out != input_len || memcmp(out2, sample, input_len) != 0) {
        inflateEnd(&istrm);
        free(cbuf1);
        free(cbuf2);
        free(out1);
        free(out2);
        return -12;
    }
    inflateEnd(&istrm);
    out2[input_len] = '\0';

    // Verify CRC again
    uLong crc_stream_uncomp = crc32(0L, Z_NULL, 0);
    crc_stream_uncomp = crc32(crc_stream_uncomp, out2, input_len);
    if (crc_stream_uncomp != crc_orig) {
        free(cbuf1);
        free(cbuf2);
        free(out1);
        free(out2);
        return -13;
    }

    // Clean up
    free(cbuf1);
    free(cbuf2);
    free(out1);
    free(out2);

    // If we reached here, all zlib API sequences succeeded
    return 66;
}