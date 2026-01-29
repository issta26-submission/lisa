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
//<ID> 887
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);

    // step 2: Compression & initialize inflate streams
    compress(comp, &compLen, orig, ORIG_LEN);
    z_stream istrm_src;
    memset(&istrm_src, 0, sizeof(istrm_src));
    istrm_src.zalloc = NULL;
    istrm_src.zfree = NULL;
    istrm_src.opaque = NULL;
    inflateInit_(&istrm_src, ver, (int)sizeof(z_stream));
    istrm_src.next_in = comp;
    istrm_src.avail_in = (uInt)compLen;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    istrm_copy.zalloc = NULL;
    istrm_copy.zfree = NULL;
    istrm_copy.opaque = NULL;
    inflateCopy(&istrm_copy, &istrm_src);

    // step 3: Operate & use gz functions (use files to avoid blocking on stdio fds)
    istrm_copy.next_out = outbuf;
    istrm_copy.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_copy, 0);
    uLong crc_decomp = crc32(0UL, outbuf, (uInt)ORIG_LEN);
    gzFile gw = gzopen("tmp_zlib_test.gz", "wb");
    gzbuffer(gw, 1024);
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_test.gz", "rb");
    gzbuffer(gr, 4096);
    int ch = gzgetc(gr);
    gzclose(gr);

    // step 4: Cleanup
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm_src);
    free(orig);
    free(comp);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)crc_decomp;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}