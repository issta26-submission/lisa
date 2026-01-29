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
//<ID> 888
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
    compress(comp, &compLen, orig, ORIG_LEN);

    // step 2: Setup inflate streams and copy
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

    // step 3: Operate & validate, write/read via gz functions
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    istrm_copy.next_out = outbuf;
    istrm_copy.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_copy, 0);
    uLong crc_decompressed = crc32(0UL, outbuf, (uInt)ORIG_LEN);
    gzFile gw = gzopen("zlib_api_tmp.gz", "wb");
    gzwrite(gw, outbuf, (unsigned int)ORIG_LEN);
    gzclose(gw);
    gzFile gr = gzopen("zlib_api_tmp.gz", "rb");
    gzbuffer(gr, 1024);
    int first_char = gzgetc(gr);
    gzclose(gr);

    // step 4: Cleanup
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)crc_decompressed;
    (void)first_char;
    // API sequence test completed successfully
    return 66;
}