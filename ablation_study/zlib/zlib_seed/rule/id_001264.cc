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
//<ID> 1264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len = 0;
    uLong decomp_len = SRC_LEN + 32;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);

    // step 2: Compress the source using compress2
    const char *ver = zlibVersion();
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    // step 3: Initialize inflate, reset keep, perform inflate, and end inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateResetKeep(&istrm);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateEnd(&istrm);

    // step 4: Write decompressed data to a gz file, seek using gzseek64, close and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzwrite(gf, decomp, (unsigned int)SRC_LEN);
    off64_t pos = gzseek64(gf, 0, 0);
    (void)pos;
    gzclose(gf);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}