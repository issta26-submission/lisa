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
//<ID> 1120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare data
    const uLong LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)LEN);
    memset(orig, 'A', (size_t)(LEN - 1));
    orig[LEN - 1] = 0;
    const char *ver = zlibVersion();
    uLong bound = compressBound(LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, LEN, 6);

    // step 2: Initialize inflate stream and reset windowBits
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset2(&istrm, 15);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;

    // step 3: Perform inflate and write output to gzip using gzputs
    Bytef *decomp = (Bytef *)malloc((size_t)LEN);
    memset(decomp, 0, (size_t)LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)LEN;
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gf, (const char *)decomp);
    gzclose(gf);

    // step 4: Cleanup
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}