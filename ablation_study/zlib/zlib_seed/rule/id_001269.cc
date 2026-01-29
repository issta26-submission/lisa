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
//<ID> 1269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it with compress2
    const uLong SRC_LEN = 200UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLong comp_len = comp_bound;
    const char *ver = zlibVersion();
    compress2(comp, (uLongf *)&comp_len, src, SRC_LEN, 6);

    // step 2: Initialize inflate stream, perform inflate, then reset keep
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    uLong decomp_len = SRC_LEN + 16;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4);
    inflateResetKeep(&istrm);

    // step 3: Create a gzip file, write original data, seek and close
    gzFile gf = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, src, (unsigned int)SRC_LEN);
    off64_t pos = gzseek64(gf, 0, 0);
    (void)pos;
    gzclose(gf);

    // step 4: Cleanup inflate stream and allocated buffers
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}