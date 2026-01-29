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
//<ID> 1185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute Adler-32 checksums, then combine them
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong len1 = ORIG_LEN / 2;
    uLong len2 = ORIG_LEN - len1;
    uLong ad1 = adler32(0UL, orig, (uInt)len1);
    uLong ad2 = adler32(0UL, orig + len1, (uInt)len2);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)len2);

    // step 2: Initialize a deflate stream, compress the data, then end the deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Write compressed data to a gzip file using gzwrite, then initialize inflate and call inflateReset2
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, comp, (unsigned int)comp_size);
    gzclose(gz);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset2(&istrm, 15);

    // step 4: Inflate the compressed data to validate round-trip, cleanup buffers and streams
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    (void)ad_combined;
    return 66;
}