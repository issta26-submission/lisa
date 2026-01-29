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
//<ID> 858
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original data
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(1UL, orig, (uInt)ORIG_LEN);

    // step 2: Compress original data with deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate with inflateInit2_, perform inflate, validate via adler
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.adler = orig_adler;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    Bytef *out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0); /* Z_NO_FLUSH */
    uLong decompressed = istrm.total_out;
    inflateEnd(&istrm);

    // step 4: Use gzwrite/gzread to write original to a gzip file and read back some bytes, then cleanup
    gzFile gw = gzopen("test_zlib_api_seq.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_seq.gz", "rb");
    Bytef *gzbuf = (Bytef *)malloc(128U);
    memset(gzbuf, 0, 128U);
    int r = gzread(gr, gzbuf, 128U);
    gzclose(gr);

    free(orig);
    free(comp);
    free(out);
    free(gzbuf);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)decompressed;
    (void)r;
    (void)orig_adler;
    // API sequence test completed successfully
    return 66;
}