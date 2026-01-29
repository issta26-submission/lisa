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
//<ID> 1202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and compute checksum
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(0UL, orig, (uInt)ORIG_LEN);

    // step 2: Compress original data using compress2
    uLong comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);

    // step 3: Decompress using uncompress and initialize/reset an inflate stream
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = (uLongf)ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, (uLong)comp_len);
    uLong decomp_adler = adler32(0UL, decomp, (uInt)decomp_len);
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateResetKeep(&istrm);

    // step 4: Write compressed data to a gzip file and cleanup resources
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, comp, (unsigned int)comp_len);
    gzclose(gz);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}