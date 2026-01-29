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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a sequence of zlib API usages:
    // - checksum calculations (crc32, adler32)
    // - simple one-shot compression/uncompression (compress2, uncompress)
    // - stream compression/decompression (deflateInit/deflate/deflateEnd + inflateInit/inflate/inflateEnd)
    //
    // On success the function returns 66. On any failure a non-66 value is returned.

    const char *input = "The quick brown fox jumps over the lazy dog. "
                        "Pack my box with five dozen liquor jugs. "
                        "Sphinx of black quartz, judge my vow. "
                        "0123456789abcdef"; // modest-size test data
    size_t input_len_sz = strlen(input);
    if (input_len_sz == 0) return 1;

    // Ensure sizes fit zlib's uLong/uInt types for this test data.
    uLong input_len = (uLong)input_len_sz;
    if ((size_t)input_len != input_len_sz) return 2;

    // 1) Compute checksums
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (const Bytef *)input, (uInt)input_len);
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, (const Bytef *)input, (uInt)input_len);

    // 2) One-shot compression using compress2
    uLong bound = compressBound(input_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    if (!comp) return 3;
    uLongf comp_len = (uLongf)bound;
    int zret = compress2(comp, &comp_len, (const Bytef *)input, input_len, Z_BEST_COMPRESSION);
    if (zret != Z_OK) { free(comp); return 4; }

    // 3) One-shot uncompress
    Bytef *uncomp = (Bytef *)malloc((size_t)input_len);
    if (!uncomp) { free(comp); return 5; }
    uLongf uncomp_len = (uLongf)input_len;
    zret = uncompress(uncomp, &uncomp_len, comp, comp_len);
    if (zret != Z_OK) { free(comp); free(uncomp); return 6; }
    if (uncomp_len != input_len) { free(comp); free(uncomp); return 7; }
    if (memcmp(uncomp, input, input_len) != 0) { free(comp); free(uncomp); return 8; }

    // 4) Stream compression (deflate) and stream decompression (inflate)
    // Prepare deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    zret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (zret != Z_OK) { free(comp); free(uncomp); return 9; }

    // We will write deflated output into a buffer of size 'bound' (should be sufficient for this sample).
    Bytef *stream_comp = (Bytef *)malloc((size_t)bound);
    if (!stream_comp) { deflateEnd(&dstrm); free(comp); free(uncomp); return 10; }
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)input_len;
    dstrm.next_out = stream_comp;
    dstrm.avail_out = (uInt)bound;

    zret = deflate(&dstrm, Z_FINISH);
    if (zret != Z_STREAM_END) {
        // If buffer was not enough, normally one would loop and expand buffer.
        // For this test data we expect it to be enough; otherwise report error.
        deflateEnd(&dstrm);
        free(comp);
        free(uncomp);
        free(stream_comp);
        return 11;
    }
    uLong stream_comp_len = (uLong)(bound - dstrm.avail_out);
    deflateEnd(&dstrm);

    // Inflate the stream-compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    zret = inflateInit(&istrm);
    if (zret != Z_OK) { free(comp); free(uncomp); free(stream_comp); return 12; }

    Bytef *stream_uncomp = (Bytef *)malloc((size_t)input_len);
    if (!stream_uncomp) { inflateEnd(&istrm); free(comp); free(uncomp); free(stream_comp); return 13; }
    istrm.next_in = stream_comp;
    istrm.avail_in = (uInt)stream_comp_len;
    istrm.next_out = stream_uncomp;
    istrm.avail_out = (uInt)input_len;

    zret = inflate(&istrm, Z_FINISH);
    if (zret != Z_STREAM_END) {
        inflateEnd(&istrm);
        free(comp);
        free(uncomp);
        free(stream_comp);
        free(stream_uncomp);
        return 14;
    }
    uLong stream_uncomp_len = (uLong)(input_len - istrm.avail_out);
    inflateEnd(&istrm);

    if (stream_uncomp_len != input_len) {
        free(comp);
        free(uncomp);
        free(stream_comp);
        free(stream_uncomp);
        return 15;
    }
    if (memcmp(stream_uncomp, input, input_len) != 0) {
        free(comp);
        free(uncomp);
        free(stream_comp);
        free(stream_uncomp);
        return 16;
    }

    // Cleanup
    free(comp);
    free(uncomp);
    free(stream_comp);
    free(stream_uncomp);

    // If we reached here, all tested zlib calls succeeded and round-trips matched.
    (void)crc; // silence unused variable warnings if any
    (void)adl;
    return 66;
}