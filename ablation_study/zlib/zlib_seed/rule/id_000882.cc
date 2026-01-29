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
//<ID> 882
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
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);

    // step 2: Compression
    compress(comp, &compLen, orig, ORIG_LEN);

    // step 3: Initialize inflate stream and perform inflateCopy, then inflate using the copy
    z_stream src;
    memset(&src, 0, sizeof(src));
    src.zalloc = NULL;
    src.zfree = NULL;
    src.opaque = NULL;
    inflateInit_(&src, ver, (int)sizeof(z_stream));
    src.next_in = comp;
    src.avail_in = (uInt)compLen;
    src.next_out = outbuf;
    src.avail_out = (uInt)ORIG_LEN;
    z_stream dest;
    memset(&dest, 0, sizeof(dest));
    dest.zalloc = NULL;
    dest.zfree = NULL;
    dest.opaque = NULL;
    inflateCopy(&dest, &src);
    inflate(&dest, 0);
    uLong crc_decomp = crc32(0UL, outbuf, (uInt)ORIG_LEN);

    // step 4: Write compressed bytes to a gzip file, set buffer sizes, read one byte back, cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gw, 8192);
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzbuffer(gr, 4096);
    int first_byte = gzgetc(gr);
    gzclose(gr);

    inflateEnd(&dest);
    inflateEnd(&src);
    free(orig);
    free(comp);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)crc_orig;
    (void)crc_decomp;
    (void)first_byte;
    // API sequence test completed successfully
    return 66;
}