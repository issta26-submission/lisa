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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original data and compressed buffer
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and perform decompression
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);

    // step 3: Write version to a gzip file, then read it back using gzfread and clear errors
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gw, ver);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    z_size_t items_read = gzfread((voidp)decomp, (z_size_t)1, (z_size_t)ORIG_LEN, gr);
    (void)items_read;
    gzclearerr(gr);
    gzclose(gr);

    // step 4: Finalize inflate, cleanup resources
    inflateEnd(&istrm);
    inflateBackEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}