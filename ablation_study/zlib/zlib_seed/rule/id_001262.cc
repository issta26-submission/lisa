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
//<ID> 1262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a zlib-compressed buffer
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLong comp_size = comp_bound;
    compress2(comp, &comp_size, src, SRC_LEN, 6);

    // step 2: Write the original data to a gzip file, reopen it and perform 64-bit seeks, then close
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzflush(gw, 0);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t pos_after = gzseek64(gr, 10, 0);
    off64_t pos_start = gzseek64(gr, 0, 0);
    int gzclose_res = gzclose(gr);
    (void)pos_after;
    (void)pos_start;
    (void)gzclose_res;

    // step 3: Initialize an inflate stream, reset keeping state, and inflate the compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    const char *ver = zlibVersion();
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateResetKeep(&istrm);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    uLong decomp_len = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4);
    inflateEnd(&istrm);

    // step 4: Cleanup buffers and finish
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}