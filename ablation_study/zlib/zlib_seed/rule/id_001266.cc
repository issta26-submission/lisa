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
//<ID> 1266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a memory buffer
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    compress2(comp, &comp_len, src, SRC_LEN, 6);
    uLong comp_size = (uLong)comp_len;

    // step 2: Write compressed data to a gzip file, seek, and close it using gzseek64 and gzclose
    gzFile gfile = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gfile, comp, (unsigned int)comp_size);
    gzflush(gfile, 0);
    off64_t pos = gzseek64(gfile, 0, 0);
    (void)pos;
    gzclose(gfile);

    // step 3: Initialize inflate stream, decompress, reset keep and decompress again
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    const char *ver = zlibVersion();
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    uLong decomp_len1 = SRC_LEN + 64;
    Bytef *decomp1 = (Bytef *)malloc((size_t)decomp_len1);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp1;
    istrm.avail_out = (uInt)decomp_len1;
    inflate(&istrm, 4); /* Z_FINISH */

    inflateResetKeep(&istrm);

    uLong decomp_len2 = SRC_LEN + 128;
    Bytef *decomp2 = (Bytef *)malloc((size_t)decomp_len2);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)decomp_len2;
    inflate(&istrm, 4); /* Z_FINISH */

    // step 4: Cleanup inflate stream and free all allocated resources
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp1);
    free(decomp2);
    // API sequence test completed successfully
    return 66;
}