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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it with compress2
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLongf comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_size = comp_bound;
    compress2(comp, &comp_size, src, SRC_LEN, 6);

    // step 2: Initialize inflate stream, perform inflate, then call inflateResetKeep
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    uLong decomp_len = SRC_LEN;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateResetKeep(&istrm);

    // step 3: Write compressed data to a gzip file using gzopen64/gzwrite, seek with gzseek64, then close with gzclose
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzf, comp, (unsigned int)comp_size);
    off64_t pos = gzseek64(gzf, 0, 0); (void)pos;
    gzclose(gzf);

    // step 4: Cleanup inflate stream and allocated buffers
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}