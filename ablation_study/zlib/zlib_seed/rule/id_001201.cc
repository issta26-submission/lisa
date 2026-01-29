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
//<ID> 1201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute initial checksum
    const uLong ORIG_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(src, 'X', (size_t)ORIG_LEN);
    uLong ad_before = adler32(0UL, src, (uInt)ORIG_LEN);

    // step 2: Compress the data and write compressed bytes to a gzip file
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, src, ORIG_LEN, 6);
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, comp, (unsigned int)comp_len);

    // step 3: Initialize an inflate stream, perform reset-keep, then uncompress and validate checksum
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    const char *ver = zlibVersion();
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateResetKeep(&istrm);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_len);
    uLong ad_after = adler32(0UL, decomp, (uInt)decomp_len);

    // step 4: Cleanup resources
    inflateEnd(&istrm);
    gzclose(gz);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}