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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    /* Include headers here so the function is self-contained when pasted alone.
       Note: Putting preprocessor directives inside a function is unusual but valid.
       If you paste this into a larger file, you can move these includes to the top. */
    #include <zlib.h>
    #include <stdlib.h>
    #include <string.h>

    int ret_code = 1; /* non-66 means failure by default */

    /* Build sample input data (repeated phrase to get a reasonable size). */
    const char *phrase = "The quick brown fox jumps over the lazy dog. ";
    const int repeats = 200; /* ~9000 bytes total which is safe for uInt */
    uLong orig_len = (uLong)(strlen(phrase) * repeats);
    unsigned char *orig = (unsigned char *)malloc(orig_len);
    if (!orig) return 1;
    for (int i = 0; i < repeats; ++i) {
        memcpy(orig + i * strlen(phrase), phrase, strlen(phrase));
    }

    /* 1) Simple API: compress() / uncompress() with compressBound() */
    uLong bound = compressBound(orig_len);
    unsigned char *comp = (unsigned char *)malloc(bound);
    if (!comp) { free(orig); return 1; }
    uLong comp_len = bound;
    int zerr = compress(comp, &comp_len, orig, orig_len);
    if (zerr != Z_OK) {
        free(orig); free(comp);
        return 1;
    }

    /* decompress with uncompress() */
    unsigned char *decomp = (unsigned char *)malloc(orig_len);
    if (!decomp) { free(orig); free(comp); return 1; }
    uLong decomp_len = orig_len;
    zerr = uncompress(decomp, &decomp_len, comp, comp_len);
    if (zerr != Z_OK || decomp_len != orig_len || memcmp(orig, decomp, orig_len) != 0) {
        free(orig); free(comp); free(decomp);
        return 1;
    }

    /* 2) Streaming API: deflateInit / deflate / deflateEnd
       We'll compress via z_stream in a loop, allowing output to grow as needed. */
    z_stream defstream;
    memset(&defstream, 0, sizeof(defstream));
    zerr = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (zerr != Z_OK) {
        free(orig); free(comp); free(decomp);
        return 1;
    }

    /* Setup streaming input */
    defstream.next_in = (Bytef *)orig;
    defstream.avail_in = (uInt)orig_len; /* safe since orig_len is modest */

    size_t out_cap = (size_t)compressBound(orig_len);
    unsigned char *stream_comp = (unsigned char *)malloc(out_cap);
    if (!stream_comp) {
        deflateEnd(&defstream);
        free(orig); free(comp); free(decomp);
        return 1;
    }
    defstream.next_out = stream_comp;
    defstream.avail_out = (uInt)out_cap;

    /* Perform deflate in a loop until Z_STREAM_END */
    while (1) {
        zerr = deflate(&defstream, Z_FINISH);
        if (zerr == Z_STREAM_END) break;
        if (zerr != Z_OK && zerr != Z_BUF_ERROR) {
            deflateEnd(&defstream);
            free(orig); free(comp); free(decomp); free(stream_comp);
            return 1;
        }
        /* Need more output space */
        size_t used = out_cap - defstream.avail_out;
        size_t new_cap = out_cap * 2;
        unsigned char *tmp = (unsigned char *)realloc(stream_comp, new_cap);
        if (!tmp) {
            deflateEnd(&defstream);
            free(orig); free(comp); free(decomp); free(stream_comp);
            return 1;
        }
        stream_comp = tmp;
        defstream.next_out = stream_comp + used;
        defstream.avail_out = (uInt)(new_cap - used);
        out_cap = new_cap;
    }
    /* compressed size from streaming deflate */
    size_t stream_comp_len = out_cap - defstream.avail_out;
    deflateEnd(&defstream);

    /* 3) Streaming inflate: inflateInit / inflate / inflateEnd */
    z_stream infstream;
    memset(&infstream, 0, sizeof(infstream));
    zerr = inflateInit(&infstream);
    if (zerr != Z_OK) {
        free(orig); free(comp); free(decomp); free(stream_comp);
        return 1;
    }

    infstream.next_in = stream_comp;
    infstream.avail_in = (uInt)stream_comp_len;

    unsigned char *stream_decomp = (unsigned char *)malloc(orig_len);
    if (!stream_decomp) {
        inflateEnd(&infstream);
        free(orig); free(comp); free(decomp); free(stream_comp);
        return 1;
    }
    infstream.next_out = stream_decomp;
    infstream.avail_out = (uInt)orig_len;

    while (1) {
        zerr = inflate(&infstream, Z_FINISH);
        if (zerr == Z_STREAM_END) break;
        if (zerr != Z_OK && zerr != Z_BUF_ERROR) {
            inflateEnd(&infstream);
            free(orig); free(comp); free(decomp); free(stream_comp); free(stream_decomp);
            return 1;
        }
        /* If we run out of output space unexpectedly, fail (we sized it to original size). */
        if (infstream.avail_out == 0) {
            inflateEnd(&infstream);
            free(orig); free(comp); free(decomp); free(stream_comp); free(stream_decomp);
            return 1;
        }
    }
    size_t stream_decomp_len = orig_len - infstream.avail_out;
    inflateEnd(&infstream);

    /* Validate streaming inflate result matches original */
    if (stream_decomp_len != orig_len || memcmp(orig, stream_decomp, orig_len) != 0) {
        free(orig); free(comp); free(decomp); free(stream_comp); free(stream_decomp);
        return 1;
    }

    /* 4) Demonstrate crc32 on original and decompressed data to validate integrity */
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, orig, (uInt)orig_len);
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, stream_decomp, (uInt)stream_decomp_len);
    if (crc_orig != crc_decomp) {
        free(orig); free(comp); free(decomp); free(stream_comp); free(stream_decomp);
        return 1;
    }

    /* Clean up all allocated memory */
    free(orig);
    free(comp);
    free(decomp);
    free(stream_comp);
    free(stream_decomp);

    /* Success: return 66 as requested */
    (void)crc_orig; /* silence unused-variable warnings if any */
    ret_code = 66;
    return ret_code;
}