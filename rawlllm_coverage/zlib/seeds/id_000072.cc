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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Example data
    const char *original = "The quick brown fox jumps over the lazy dog. "
                           "This is a test string for zlib compression and decompression.";
    uLong source_len = (uLong)strlen(original);

    // Basic check: version string
    const char *ver = zlibVersion();
    if (!ver || ver[0] == '\0') return -1;

    // 1) Convenience API: compress() / uncompress()
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc(bound);
    if (!comp_buf) return -2;
    uLongf comp_len = bound;
    int ret = compress(comp_buf, &comp_len, (const Bytef *)original, source_len);
    if (ret != Z_OK) { free(comp_buf); return -3; }

    // Uncompress with known destination size
    Bytef *uncomp_buf = (Bytef *)malloc(source_len + 1);
    if (!uncomp_buf) { free(comp_buf); return -4; }
    uLongf uncomp_len = source_len;
    ret = uncompress(uncomp_buf, &uncomp_len, comp_buf, comp_len);
    if (ret != Z_OK || uncomp_len != source_len) { free(comp_buf); free(uncomp_buf); return -5; }
    uncomp_buf[uncomp_len] = '\0';
    if (memcmp(uncomp_buf, original, source_len) != 0) { free(comp_buf); free(uncomp_buf); return -6; }

    // 2) Streaming API: deflateInit / deflate / deflateEnd
    z_stream defstream;
    memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) { free(comp_buf); free(uncomp_buf); return -7; }

    // prepare output buffer large enough (compressBound)
    Bytef *stream_comp = (Bytef *)malloc(bound);
    if (!stream_comp) { deflateEnd(&defstream); free(comp_buf); free(uncomp_buf); return -8; }

    defstream.next_in = (Bytef *)original;
    defstream.avail_in = source_len;
    defstream.next_out = stream_comp;
    defstream.avail_out = bound;

    // Perform deflate with Z_FINISH. With a large enough buffer this should finish in one call.
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) { deflateEnd(&defstream); free(stream_comp); free(comp_buf); free(uncomp_buf); return -9; }
    uLong stream_comp_len = (uLong)(bound - defstream.avail_out);

    // clean up deflate
    deflateEnd(&defstream);

    // 3) Streaming API: inflateInit / inflate / inflateEnd
    z_stream infstream;
    memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) { free(stream_comp); free(comp_buf); free(uncomp_buf); return -10; }

    // inflate into buffer sized source_len + 1
    Bytef *stream_uncomp = (Bytef *)malloc(source_len + 1);
    if (!stream_uncomp) { inflateEnd(&infstream); free(stream_comp); free(comp_buf); free(uncomp_buf); return -11; }

    infstream.next_in = stream_comp;
    infstream.avail_in = stream_comp_len;
    infstream.next_out = stream_uncomp;
    infstream.avail_out = source_len;

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) { inflateEnd(&infstream); free(stream_uncomp); free(stream_comp); free(comp_buf); free(uncomp_buf); return -12; }
    uLong stream_uncomp_len = (uLong)(source_len - infstream.avail_out);

    inflateEnd(&infstream);

    // verify streaming inflate result matches original
    stream_uncomp[stream_uncomp_len] = '\0';
    if (stream_uncomp_len != source_len || memcmp(stream_uncomp, original, source_len) != 0) {
        free(stream_uncomp); free(stream_comp); free(comp_buf); free(uncomp_buf);
        return -13;
    }

    // 4) Checksums: crc32 and adler32
    uLong crc_orig = crc32(0L, (const Bytef *)original, source_len);
    uLong crc_uncomp = crc32(0L, (const Bytef *)stream_uncomp, stream_uncomp_len);
    if (crc_orig != crc_uncomp) { free(stream_uncomp); free(stream_comp); free(comp_buf); free(uncomp_buf); return -14; }

    uLong adl_orig = adler32(0L, (const Bytef *)original, source_len);
    uLong adl_uncomp = adler32(0L, (const Bytef *)stream_uncomp, stream_uncomp_len);
    if (adl_orig != adl_uncomp) { free(stream_uncomp); free(stream_comp); free(comp_buf); free(uncomp_buf); return -15; }

    // All tests passed; free memory and return success code 66
    free(stream_uncomp);
    free(stream_comp);
    free(comp_buf);
    free(uncomp_buf);

    return 66;
}