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
//<ID> 1188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute checksums
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong ad1 = adler32(1UL, orig, (uInt)(ORIG_LEN / 2));
    uLong ad2 = adler32(1UL, orig + (ORIG_LEN / 2), (uInt)(ORIG_LEN - (ORIG_LEN / 2)));
    uLong combined = adler32_combine(ad1, ad2, (off_t)(ORIG_LEN / 2));

    // step 2: Initialize and configure streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    int windowBits = (int)(combined % 15) + 15;
    inflateReset2(&istrm, windowBits);

    // step 3: Operate (compress using deflate, write compressed data and version to gzip)
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);

    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, ver, (unsigned int)strlen(ver));
    gzwrite(gz, comp, (unsigned int)dstrm.total_out);
    gzclose(gz);

    // step 4: Validate and cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    // API sequence test completed successfully
    return 66;
}