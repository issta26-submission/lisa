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
//<ID> 881
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outbuf1 = (Bytef *)malloc((size_t)ORIG_LEN);
    Bytef *outbuf2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf1, 0, (size_t)ORIG_LEN);
    memset(outbuf2, 0, (size_t)ORIG_LEN);

    // step 2: Setup: compress data and initialize inflate streams
    compress(comp, &compLen, orig, ORIG_LEN);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    z_stream copystrm;
    memset(&copystrm, 0, sizeof(copystrm));
    inflateCopy(&copystrm, &istrm);
    copystrm.zalloc = NULL;
    copystrm.zfree = NULL;
    copystrm.opaque = NULL;

    // step 3: Operate & validate
    istrm.next_out = outbuf1;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    copystrm.next_out = outbuf2;
    copystrm.avail_out = (uInt)ORIG_LEN;
    inflate(&copystrm, 0);
    uLong crc1 = crc32(0UL, outbuf1, (uInt)ORIG_LEN);
    uLong crc2 = crc32(0UL, outbuf2, (uInt)ORIG_LEN);
    gzFile gw = gzdopen(1, "wb");
    gzbuffer(gw, 4096U);
    gzwrite(gw, comp, (unsigned int)compLen);
    int ch = gzgetc(gw);
    gzclose(gw);

    // step 4: Cleanup
    inflateEnd(&istrm);
    inflateEnd(&copystrm);
    free(orig);
    free(comp);
    free(outbuf1);
    free(outbuf2);
    (void)ver;
    (void)bound;
    (void)crc1;
    (void)crc2;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}