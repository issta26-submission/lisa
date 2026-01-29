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
//<ID> 1265
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
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, src, SRC_LEN, 6);
    uLong decomp_len = SRC_LEN + 16;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);

    // step 2: Initialize and configure inflate stream
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Operate: inflate the compressed buffer, reset keep, and finalize inflate
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateResetKeep(&istrm);
    inflateEnd(&istrm);

    // step 4: Create a gzip file, write original data, seek using gzseek64, and close
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzflush(gw, 0);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t pos = gzseek64(gr, (off64_t)0, 0);
    (void)pos;
    char peek[8];
    gzread(gr, peek, 8);
    gzclose(gr);

    // cleanup
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}