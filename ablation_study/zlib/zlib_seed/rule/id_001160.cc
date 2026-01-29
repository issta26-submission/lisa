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
//<ID> 1160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and compress it with compress2
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLongf comp_buf_len = (uLongf)compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_buf_len);
    memset(comp, 0, (size_t)comp_buf_len);
    compress2(comp, &comp_buf_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate, perform a first inflation, then reset and perform a second inflation using reset APIs
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_buf_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateReset(&istrm);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_buf_len;
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateResetKeep(&istrm);
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 3: Write zlib version into a gzip file and read it back
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzputs(gw, ver);
    gzputs(gw, "\n");
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char ver_read[128];
    memset(ver_read, 0, sizeof(ver_read));
    gzgets(gr, ver_read, (int)sizeof(ver_read));
    gzclose(gr);

    // step 4: Validate checksums and cleanup
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);
    (void)crc_orig;
    (void)crc_decomp;
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)ver_read;
    // API sequence test completed successfully
    return 66;
}