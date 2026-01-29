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
//<ID> 1260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress with compress2
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLongf comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len = comp_bound;
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    // step 2: Create a gzip file, write original data, flush, seek and close
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzf, src, (unsigned int)SRC_LEN);
    gzflush(gzf, 0);
    off64_t seek_pos = gzseek64(gzf, 0, 0);
    (void)seek_pos;
    gzclose(gzf);

    // step 3: Initialize inflate stream, perform inflate, then reset keep and end
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
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4);
    inflateResetKeep(&istrm);
    inflateEnd(&istrm);

    // step 4: Cleanup allocated buffers and finalize
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}