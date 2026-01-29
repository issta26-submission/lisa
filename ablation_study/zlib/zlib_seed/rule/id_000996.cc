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
//<ID> 996
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong comp_bound = compressBound(ORIG_LEN);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress data and initialize inflate stream
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateReset(&istrm);

    // step 3: Perform inflate, compute CRCs, write original data to gzip file
    inflate(&istrm, 0);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc_combined = crc32_combine(crc_orig, crc_decomp, (off_t)ORIG_LEN);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_orig;
    (void)crc_decomp;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}