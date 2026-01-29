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
//<ID> 1187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial data
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong half_len = (uLong)(ORIG_LEN / 2);

    // compute adler32 for two halves and combine
    uLong a1 = adler32(0L, orig, (uInt)half_len);
    uLong a2 = adler32(0L, orig + half_len, (uInt)half_len);
    uLong a_combined = adler32_combine(a1, a2, (off_t)half_len);

    // step 2: Setup compression buffer and initialize streams
    uLongf comp_bound = (uLongf)compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_sizef = comp_bound;
    compress2(comp, &comp_sizef, orig, ORIG_LEN, 6);

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

    // step 3: Operate — write compressed data to a gzip file, inflate it, reset with new windowBits and inflate again
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, comp, (unsigned int)comp_sizef);

    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_sizef;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);

    // change inflate window size and perform another inflate pass with the same data
    inflateReset2(&istrm, 15);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_sizef;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    gzclose(gz);
    free(orig);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}