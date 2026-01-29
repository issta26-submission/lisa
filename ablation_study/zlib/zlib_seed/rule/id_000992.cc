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
//<ID> 992
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef *readBuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readBuf, 0, (size_t)ORIG_LEN);

    // step 2: Compute CRC of original and compress in-memory
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);

    // step 3: Write original data to a gzip file using gzwrite and then initialize inflate and inflateReset
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);
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
    inflate(&istrm, 0);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    inflateEnd(&istrm);

    // step 4: Read back from gzip file, combine CRCs, cleanup and finish
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int bytes_read = gzread(gr, readBuf, (unsigned int)ORIG_LEN);
    gzclose(gr);
    uLong crc_read = crc32(0L, readBuf, (uInt)ORIG_LEN);
    uLong crc_combined = crc32_combine(crc_read, crc_decomp, (off_t)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    free(readBuf);
    (void)ver;
    (void)crc_orig;
    (void)crc_decomp;
    (void)crc_read;
    (void)crc_combined;
    (void)bytes_read;
    // API sequence test completed successfully
    return 66;
}