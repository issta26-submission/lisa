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
//<ID> 1263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it with compress2
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    // step 2: Initialize inflate stream, perform inflate, then reset while keeping state
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    uLong decomp_len = SRC_LEN + 64UL;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateResetKeep(&istrm);

    // step 3: Create a gzip file, write original data, seek and close using gzseek64 and gzclose
    gzFile gf = gzopen64("test_zlib_api_sequence.gz", "wb+");
    gzwrite(gf, src, (unsigned int)SRC_LEN);
    gzseek64(gf, (off64_t)0, 0); /* SEEK_SET == 0 */
    gzclose(gf);

    // step 4: Cleanup inflate stream and free buffers
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}