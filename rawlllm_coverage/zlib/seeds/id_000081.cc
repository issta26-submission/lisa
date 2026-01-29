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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Example data to compress/decompress
    const char *input = "The quick brown fox jumps over the lazy dog";
    uLong input_len = (uLong)strlen(input) + 1; // include terminating NUL

    // 1) Compute CRC32 of the input
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (const Bytef *)input, input_len);
    // basic sanity: crc should be non-zero for non-empty input
    if (crc == 0) return 1;

    // 2) Use the convenience compress()/uncompress() API
    uLong comp_bound = compressBound(input_len);
    Bytef *comp_buf = (Bytef *)malloc(comp_bound);
    if (!comp_buf) return 2;
    uLong comp_size = comp_bound;
    int ret = compress(comp_buf, &comp_size, (const Bytef *)input, input_len);
    if (ret != Z_OK) { free(comp_buf); return 3; }

    Bytef *uncomp_buf = (Bytef *)malloc(input_len);
    if (!uncomp_buf) { free(comp_buf); return 4; }
    uLong uncompr_len = input_len;
    ret = uncompress(uncomp_buf, &uncompr_len, comp_buf, comp_size);
    if (ret != Z_OK) { free(comp_buf); free(uncomp_buf); return 5; }
    if (uncompr_len != input_len || memcmp(uncomp_buf, input, input_len) != 0) {
        free(comp_buf); free(uncomp_buf); return 6;
    }
    free(comp_buf);
    free(uncomp_buf);

    // 3) Use the streaming deflate/inflate APIs (z_stream)
    // Deflate (compress) with z_stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = Z_NULL;
    dstrm.zfree = Z_NULL;
    dstrm.opaque = Z_NULL;

    ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 7;

    // prepare output buffer large enough
    uLong stream_comp_bound = compressBound(input_len);
    Bytef *stream_comp = (Bytef *)malloc(stream_comp_bound);
    if (!stream_comp) { deflateEnd(&dstrm); return 8; }

    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = input_len;
    dstrm.next_out = stream_comp;
    dstrm.avail_out = (uInt)stream_comp_bound;

    // perform deflate until finish
    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        free(stream_comp);
        return 9;
    }
    uLong stream_comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // Inflate (decompress) with z_stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = Z_NULL;
    istrm.zfree = Z_NULL;
    istrm.opaque = Z_NULL;

    ret = inflateInit(&istrm);
    if (ret != Z_OK) { free(stream_comp); return 10; }

    Bytef *stream_uncomp = (Bytef *)malloc(input_len);
    if (!stream_uncomp) { inflateEnd(&istrm); free(stream_comp); return 11; }

    istrm.next_in = stream_comp;
    istrm.avail_in = (uInt)stream_comp_size;
    istrm.next_out = stream_uncomp;
    istrm.avail_out = (uInt)input_len;

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istrm);
        free(stream_comp);
        free(stream_uncomp);
        return 12;
    }
    uLong stream_uncomp_size = istrm.total_out;
    inflateEnd(&istrm);

    if (stream_uncomp_size != input_len || memcmp(stream_uncomp, input, input_len) != 0) {
        free(stream_comp);
        free(stream_uncomp);
        return 13;
    }

    free(stream_comp);
    free(stream_uncomp);

    // If all operations succeeded, return 66 as requested
    return 66;
}