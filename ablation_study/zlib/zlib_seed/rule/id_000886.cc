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
//<ID> 886
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data and initialize inflate source stream
    compress(comp, &compLen, orig, ORIG_LEN);
    z_stream istrm_src;
    memset(&istrm_src, 0, sizeof(istrm_src));
    istrm_src.zalloc = NULL;
    istrm_src.zfree = NULL;
    istrm_src.opaque = NULL;
    inflateInit_(&istrm_src, ver, (int)sizeof(z_stream));
    istrm_src.next_in = comp;
    istrm_src.avail_in = (uInt)compLen;
    istrm_src.next_out = decomp;
    istrm_src.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_src, 0);

    // step 3: Make a copy of the inflate stream and continue decompression on the copy
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm_src);
    Bytef *decomp_copy_out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_copy_out, 0, (size_t)ORIG_LEN);
    istrm_copy.next_out = decomp_copy_out;
    istrm_copy.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_copy, 0);
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm_src);

    // step 4: Write compressed data to a gz file with buffering, then read one byte back and cleanup
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzbuffer(gw, 8192U);
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c = gzgetc(gr);
    (void)c;
    gzclose(gr);

    free(orig);
    free(comp);
    free(decomp);
    free(decomp_copy_out);
    (void)ver;
    (void)bound;
    (void)compLen;
    // API sequence test completed successfully
    return 66;
}