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
//<ID> 1125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)LEN);
    memset(orig, 'X', (size_t)(LEN - 1));
    orig[LEN - 1] = 0;
    const char *ver = zlibVersion();
    uLong bound = compressBound(LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, LEN, 6);

    // step 2: Initialize inflate stream and configure
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)LEN);
    memset(decomp, 0, (size_t)LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)LEN;
    inflateReset2(&istrm, 15);

    // step 3: Operate (inflate) and finalize inflate stream
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Write decompressed data to gzip file and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gf, (const char *)decomp);
    gzclose(gf);
    free(orig);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}