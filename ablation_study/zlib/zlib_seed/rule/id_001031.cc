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
//<ID> 1031
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'G', (size_t)ORIG_LEN);
    uLong checksum_orig = adler32(1UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize and run deflate (compress) with a reset-keep call
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate (decompress) and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Write original to a gzip file, then read back a byte and push it back using gzungetc/gzgetc, compute checksum and cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int c1 = gzgetc(gr);
    gzungetc(c1, gr);
    int c2 = gzgetc(gr);
    gzclose(gr);
    uLong checksum_decomp = adler32(1UL, decomp, (uInt)ORIG_LEN);

    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)comp_size;
    (void)checksum_orig;
    (void)checksum_decomp;
    (void)c1;
    (void)c2;
    // API sequence test completed successfully
    return 66;
}