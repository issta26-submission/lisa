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
//<ID> 1124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare data, compress original string
    const uLong ORIG_LEN = 32UL;
    Bytef *orig = (Bytef *)malloc((size_t)(ORIG_LEN + 1));
    memset(orig, 0, (size_t)(ORIG_LEN + 1));
    memcpy(orig, "Hello zlib API sequence test.", 29);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN + 1);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, (uLong)(ORIG_LEN + 1), 6);

    // step 2: Initialize inflate stream, reset windowBits, and inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)(ORIG_LEN + 1));
    memset(decomp, 0, (size_t)(ORIG_LEN + 1));
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)(ORIG_LEN + 1);
    inflateReset2(&istrm, 15);
    inflate(&istrm, 4);

    // step 3: Write decompressed C-string to a gzip file using gzputs
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gf, (const char *)decomp);
    gzclose(gf);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}