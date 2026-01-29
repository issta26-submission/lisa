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
//<ID> 884
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outbuf1 = (Bytef *)malloc((size_t)ORIG_LEN);
    Bytef *outbuf2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf1, 0, (size_t)ORIG_LEN);
    memset(outbuf2, 0, (size_t)ORIG_LEN);

    // step 2: Compress source into memory
    compress(comp, &compLen, orig, ORIG_LEN);

    // step 3: Initialize inflate stream, copy it, and inflate using the copy
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;

    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm);

    istrm_copy.next_out = outbuf2;
    istrm_copy.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_copy, 0);

    // step 4: Persist compressed data to a file with gzbuffer and read one byte with gzgetc, then cleanup
    const char *fname = "tmp_zlib_test.gz";
    gzFile gw = gzopen(fname, "wb");
    gzbuffer(gw, 8192);
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    gzFile gr = gzopen(fname, "rb");
    gzbuffer(gr, 4096);
    int first_byte = gzgetc(gr);
    gzclose(gr);
    remove(fname);

    inflateEnd(&istrm_copy);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(outbuf1);
    free(outbuf2);
    (void)ver;
    (void)bound;
    (void)compLen;
    (void)first_byte;
    // API sequence test completed successfully
    return 66;
}