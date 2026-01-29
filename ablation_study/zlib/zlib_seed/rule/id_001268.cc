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
//<ID> 1268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress to a memory buffer
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    compress2(comp, &comp_len_f, src, SRC_LEN, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Write the compressed data into a gzip file, seek to start, and close the file
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_len);
    gzflush(gf, 0);
    gzseek64(gf, (off64_t)0, 0);
    gzclose(gf);

    // step 3: Initialize an inflate stream, reset keep, perform inflate (decompress) into a buffer
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateResetKeep(&istrm);
    Bytef *decomp = (Bytef *)malloc((size_t)SRC_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)SRC_LEN;
    inflate(&istrm, 4);

    // step 4: Cleanup inflate stream, close any remaining resources and free buffers
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}