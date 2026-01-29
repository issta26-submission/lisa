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
//<ID> 880
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
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
    Bytef *out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out, 0, (size_t)ORIG_LEN);
    copystrm.next_out = out;
    copystrm.avail_out = (uInt)ORIG_LEN;
    inflate(&copystrm, 0);

    gzFile gw = gzopen("tmp_zlib_test.gz", "wb");
    gzbuffer(gw, 16384U);
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_test.gz", "rb");
    int ch = gzgetc(gr);
    (void)ch;
    gzclose(gr);

    inflateEnd(&copystrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(out);
    (void)ver;
    (void)bound;
    return 66;
}