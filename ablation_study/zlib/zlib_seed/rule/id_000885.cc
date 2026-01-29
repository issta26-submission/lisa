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
//<ID> 885
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
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
    inflateCopy(&istrm_copy, &istrm_src);
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    istrm_copy.next_out = outbuf;
    istrm_copy.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_copy, 0);
    uLong crc_decompressed = crc32(0UL, outbuf, (uInt)ORIG_LEN);

    FILE *tf = tmpfile();
    int fd = fileno(tf);
    int fdw = dup(fd);
    gzFile gw = gzdopen(fdw, "wb");
    gzbuffer(gw, 4096);
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    fflush(tf);
    fseek(tf, 0, SEEK_SET);
    int fdr = fileno(tf);
    int fdrdup = dup(fdr);
    gzFile gr = gzdopen(fdrdup, "rb");
    gzbuffer(gr, 2048);
    int got = gzgetc(gr);
    gzclose(gr);
    fclose(tf);

    inflateEnd(&istrm_copy);
    inflateEnd(&istrm_src);
    free(orig);
    free(comp);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)crc_decompressed;
    (void)got;
    return 66;
}