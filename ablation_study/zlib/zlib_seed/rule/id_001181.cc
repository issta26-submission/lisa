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
//<ID> 1181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare data
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLongf comp_bound = (uLongf)compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;

    // step 2: Initialize deflate stream and compress data
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;

    // step 3: Write compressed data to a gzip file using gzwrite and close it
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, comp, (unsigned int)comp_size);
    gzclose(gz);

    // step 4: Initialize inflate stream, adjust window, inflate, combine checksums, cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset2(&istrm, 15);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    uLong ad1 = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong ad2 = adler32(0L, decomp, (uInt)ORIG_LEN);
    uLong combined = adler32_combine(ad1, ad2, (off_t)ORIG_LEN);
    (void)combined;
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}