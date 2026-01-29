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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a full zlib stream compression -> decompression
    // cycle using deflate/inflate and verifies the round-trip integrity.
    const char *source = "The quick brown fox jumps over the lazy dog";
    unsigned long source_len = (unsigned long)strlen(source);

    // --- Compression ---
    z_stream defstream;
    memset(&defstream, 0, sizeof(defstream));
    int err = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (err != Z_OK) return 1;

    // Allocate an output buffer large enough for compressed data
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef*)malloc(bound);
    if (!comp_buf) { deflateEnd(&defstream); return 2; }

    defstream.next_in = (Bytef*)source;
    defstream.avail_in = (uInt)source_len; // input is small here, cast is safe
    defstream.next_out = comp_buf;
    defstream.avail_out = (uInt)bound;

    err = deflate(&defstream, Z_FINISH);
    if (err != Z_STREAM_END) { deflateEnd(&defstream); free(comp_buf); return 3; }
    uLong comp_size = defstream.total_out;
    deflateEnd(&defstream);

    // --- Decompression ---
    z_stream infstream;
    memset(&infstream, 0, sizeof(infstream));
    err = inflateInit(&infstream);
    if (err != Z_OK) { free(comp_buf); return 4; }

    // Allocate expected output buffer (we know original size)
    Bytef *decomp_buf = (Bytef*)malloc(source_len + 1);
    if (!decomp_buf) { inflateEnd(&infstream); free(comp_buf); return 5; }

    infstream.next_in = comp_buf;
    infstream.avail_in = (uInt)comp_size;
    infstream.next_out = decomp_buf;
    infstream.avail_out = (uInt)source_len;

    err = inflate(&infstream, Z_FINISH);
    if (err != Z_STREAM_END) { inflateEnd(&infstream); free(comp_buf); free(decomp_buf); return 6; }
    uLong decomp_size = infstream.total_out;
    inflateEnd(&infstream);

    // Verify decompressed data matches original
    int ok = 0;
    if (decomp_size != source_len) ok = 7;
    else if (memcmp(decomp_buf, source, source_len) != 0) ok = 8;

    free(comp_buf);
    free(decomp_buf);

    if (ok != 0) return 9;

    // Success: return 66 as requested
    return 66;
}