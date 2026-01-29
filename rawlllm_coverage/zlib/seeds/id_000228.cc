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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function exercises multiple zlib APIs:
    //  - compress2 / uncompress
    //  - deflateInit / deflate / deflateEnd
    //  - inflateInit / inflate / inflateEnd
    //  - deflateReset / inflateReset (indirectly shown by re-init)
    //  - adler32 / crc32
    //
    // Returns 66 on success; non-66 on failure.

    const char *original = "The quick brown fox jumps over the lazy dog. "
                           "This is a test string to verify zlib API calls work correctly. "
                           "We repeat the sentence to make data larger and compressible. "
                           "The quick brown fox jumps over the lazy dog. "
                           "The quick brown fox jumps over the lazy dog.";
    uLong orig_len = (uLong)strlen(original);

    Bytef *comp = NULL;
    Bytef *un = NULL;
    Bytef *dcomp = NULL;
    Bytef *recovered = NULL;
    Bytef *out2 = NULL;
    Bytef *rec2 = NULL;

    int ret = -1; // default failure

    // 1) compress2 + uncompress
    {
        uLong bound = compressBound(orig_len);
        comp = (Bytef*)malloc(bound);
        if (!comp) goto cleanup;

        uLong comp_len = bound;
        int r = compress2(comp, &comp_len, (const Bytef*)original, orig_len, Z_BEST_COMPRESSION);
        if (r != Z_OK) goto cleanup;

        // uncompress back
        un = (Bytef*)malloc(orig_len);
        if (!un) goto cleanup;
        uLong un_len = orig_len;
        r = uncompress(un, &un_len, comp, comp_len);
        if (r != Z_OK) goto cleanup;
        if (un_len != orig_len) goto cleanup;
        if (memcmp(un, original, orig_len) != 0) goto cleanup;

        free(un);
        un = NULL;
    }

    // 2) deflate (z_stream) -> inflate (z_stream)
    {
        z_stream dstream;
        memset(&dstream, 0, sizeof(dstream));
        dstream.next_in = (Bytef*)original;
        dstream.avail_in = orig_len;

        int r = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
        if (r != Z_OK) goto cleanup;

        uLong d_bound = compressBound(orig_len);
        dcomp = (Bytef*)malloc(d_bound);
        if (!dcomp) { deflateEnd(&dstream); goto cleanup; }
        dstream.next_out = dcomp;
        dstream.avail_out = d_bound;

        r = deflate(&dstream, Z_FINISH);
        if (r != Z_STREAM_END) { deflateEnd(&dstream); goto cleanup; }
        uLong dcomp_len = d_bound - dstream.avail_out;

        deflateEnd(&dstream);

        // inflate
        z_stream istream;
        memset(&istream, 0, sizeof(istream));
        istream.next_in = dcomp;
        istream.avail_in = dcomp_len;

        r = inflateInit(&istream);
        if (r != Z_OK) goto cleanup;

        recovered = (Bytef*)malloc(orig_len);
        if (!recovered) { inflateEnd(&istream); goto cleanup; }

        istream.next_out = recovered;
        istream.avail_out = orig_len;

        r = inflate(&istream, Z_FINISH);
        if (r != Z_STREAM_END) { inflateEnd(&istream); goto cleanup; }
        uLong rec_len = orig_len - istream.avail_out;
        inflateEnd(&istream);

        if (rec_len != orig_len) goto cleanup;
        if (memcmp(recovered, original, orig_len) != 0) goto cleanup;

        free(recovered);
        recovered = NULL;
        free(dcomp);
        dcomp = NULL;
    }

    // 3) adler32 and crc32 checksums
    {
        uLong ad = adler32(0L, Z_NULL, 0);
        ad = adler32(ad, (const Bytef*)original, orig_len);
        (void)ad; // value used just to exercise API

        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, (const Bytef*)original, orig_len);
        (void)crc;
    }

    // 4) deflateReset demonstration: init, reset, deflate, inflate
    {
        z_stream d2;
        memset(&d2, 0, sizeof(d2));
        d2.next_in = (Bytef*)original;
        d2.avail_in = orig_len;

        int r = deflateInit(&d2, Z_DEFAULT_COMPRESSION);
        if (r != Z_OK) goto cleanup;

        // Reset (should succeed)
        r = deflateReset(&d2);
        if (r != Z_OK) { deflateEnd(&d2); goto cleanup; }

        uLong bound2 = compressBound(orig_len);
        out2 = (Bytef*)malloc(bound2);
        if (!out2) { deflateEnd(&d2); goto cleanup; }
        d2.next_out = out2;
        d2.avail_out = bound2;

        r = deflate(&d2, Z_FINISH);
        if (r != Z_STREAM_END) { deflateEnd(&d2); goto cleanup; }
        uLong out2_len = bound2 - d2.avail_out;
        deflateEnd(&d2);

        z_stream i2;
        memset(&i2, 0, sizeof(i2));
        i2.next_in = out2;
        i2.avail_in = out2_len;

        r = inflateInit(&i2);
        if (r != Z_OK) goto cleanup;

        rec2 = (Bytef*)malloc(orig_len);
        if (!rec2) { inflateEnd(&i2); goto cleanup; }
        i2.next_out = rec2;
        i2.avail_out = orig_len;

        r = inflate(&i2, Z_FINISH);
        if (r != Z_STREAM_END) { inflateEnd(&i2); goto cleanup; }
        inflateEnd(&i2);

        if (memcmp(rec2, original, orig_len) != 0) goto cleanup;

        free(out2);
        out2 = NULL;
        free(rec2);
        rec2 = NULL;
    }

    // If we reached here, all operations succeeded.
    ret = 66;

cleanup:
    if (comp) free(comp);
    if (un) free(un);
    if (dcomp) free(dcomp);
    if (recovered) free(recovered);
    if (out2) free(out2);
    if (rec2) free(rec2);

    return ret;
}