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
//<ID> 1267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLong comp_len = comp_bound;
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    // step 2: Initialize an inflate stream, inflate the compressed buffer, then reset-keep and inflate again
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *decomp1 = (Bytef *)malloc((size_t)(SRC_LEN + 64));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp1;
    istrm.avail_out = (uInt)(SRC_LEN + 64);
    inflate(&istrm, 4);
    inflateResetKeep(&istrm);
    Bytef *decomp2 = (Bytef *)malloc((size_t)(SRC_LEN + 64));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)(SRC_LEN + 64);
    inflate(&istrm, 4);

    // step 3: Create a gzip file, write the compressed buffer, seek with gzseek64, and close with gzclose
    gzFile gf = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_len);
    gzflush(gf, 0);
    gzseek64(gf, 0LL, 0);
    gzclose(gf);

    // step 4: Cleanup inflate stream and allocated buffers
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp1);
    free(decomp2);
    // API sequence test completed successfully
    return 66;
}