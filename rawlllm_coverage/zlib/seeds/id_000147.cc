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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Simple test data
    const char *src = "The quick brown fox jumps over the lazy dog. "
                      "Zlib API sequence test data. 0123456789";
    uLong src_len = (uLong)strlen(src);

    // 1) Use compress() and uncompress() with compressBound()
    uLong bound = compressBound(src_len);
    Bytef *comp = (Bytef *)malloc(bound);
    if (!comp) return 1;

    uLongf comp_len = bound;
    int zres = compress(comp, &comp_len, (const Bytef *)src, src_len);
    if (zres != Z_OK) { free(comp); return 2; }

    // Compute adler32 and crc32 on the original data
    uLong ad = adler32(0L, Z_NULL, 0);
    ad = adler32(ad, (const Bytef *)src, src_len);
    uLong cr = crc32(0L, Z_NULL, 0);
    cr = crc32(cr, (const Bytef *)src, src_len);

    // Uncompress and verify
    Bytef *un = (Bytef *)malloc(src_len);
    if (!un) { free(comp); return 3; }
    uLongf un_len = src_len;
    zres = uncompress(un, &un_len, comp, comp_len);
    if (zres != Z_OK || un_len != src_len || memcmp(un, src, src_len) != 0) {
        free(comp); free(un);
        return 4;
    }

    // 2) Use deflate/inflate (stream API) for incremental-style operations
    uLongf def_bound = compressBound(src_len);
    Bytef *defout = (Bytef *)malloc(def_bound);
    if (!defout) { free(comp); free(un); return 5; }

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    if (deflateInit(&dstrm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(comp); free(un); free(defout);
        return 6;
    }

    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = src_len;
    dstrm.next_out = defout;
    dstrm.avail_out = def_bound;

    zres = deflate(&dstrm, Z_FINISH);
    if (zres != Z_STREAM_END) {
        deflateEnd(&dstrm);
        free(comp); free(un); free(defout);
        return 7;
    }
    uLongf deflated_size = (uLongf)(def_bound - dstrm.avail_out);
    deflateEnd(&dstrm);

    // Inflate the deflated data
    Bytef *inflated = (Bytef *)malloc(src_len);
    if (!inflated) { free(comp); free(un); free(defout); return 8; }

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.next_in = defout;
    istrm.avail_in = deflated_size;
    istrm.next_out = inflated;
    istrm.avail_out = src_len;

    if (inflateInit(&istrm) != Z_OK) {
        free(comp); free(un); free(defout); free(inflated);
        return 9;
    }

    zres = inflate(&istrm, Z_FINISH);
    if (zres != Z_STREAM_END || istrm.total_out != src_len || memcmp(inflated, src, src_len) != 0) {
        inflateEnd(&istrm);
        free(comp); free(un); free(defout); free(inflated);
        return 10;
    }
    inflateEnd(&istrm);

    // 3) Query zlib version (sanity check)
    const char *ver = zlibVersion();
    if (!ver) {
        free(comp); free(un); free(defout); free(inflated);
        return 11;
    }

    // Cleanup
    free(comp);
    free(un);
    free(defout);
    free(inflated);

    // If we got here, all sequences succeeded
    return 66;
}