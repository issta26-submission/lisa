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
//<ID> 883
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the original data into comp using compress()
    compress(comp, &compLen, orig, ORIG_LEN);

    // step 3: Initialize inflate stream, copy it with inflateCopy, and inflate into two outputs
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *out1 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out1, 0, (size_t)ORIG_LEN);
    istrm.next_out = out1;
    istrm.avail_out = (uInt)ORIG_LEN;
    z_stream copystrm;
    memset(&copystrm, 0, sizeof(copystrm));
    inflateCopy(&copystrm, &istrm);
    Bytef *out2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out2, 0, (size_t)ORIG_LEN);
    copystrm.next_out = out2;
    copystrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflate(&copystrm, 0);

    // step 4: Write compressed data to a gz file with buffering and read one byte back with gzgetc; Cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gw, 65536U);
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzbuffer(gr, 8192U);
    int c = gzgetc(gr);
    (void)c;
    gzclose(gr);
    inflateEnd(&istrm);
    inflateEnd(&copystrm);
    free(orig);
    free(comp);
    free(out1);
    free(out2);
    (void)ver;
    (void)bound;
    (void)compLen;
    // API sequence test completed successfully
    return 66;
}