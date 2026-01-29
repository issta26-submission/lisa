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
//<ID> 975
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize inflate stream and decompress compressed buffer
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
    inflate(&istrm, 0);
    inflateValidate(&istrm, 1);
    inflateEnd(&istrm);

    // step 3: Write original data via gzFile APIs and read it back
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gz_read_buf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(gz_read_buf, 0, (size_t)ORIG_LEN);
    int read_bytes = gzread(gr, gz_read_buf, (unsigned int)ORIG_LEN);
    gzclose(gr);

    // step 4: Validate and cleanup
    uLong ad_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong ad_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    uLong ad_gz = adler32(0L, gz_read_buf, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    free(gz_read_buf);
    (void)ver;
    (void)comp_len;
    (void)read_bytes;
    (void)ad_orig;
    (void)ad_decomp;
    (void)ad_gz;
    // API sequence test completed successfully
    return 66;
}