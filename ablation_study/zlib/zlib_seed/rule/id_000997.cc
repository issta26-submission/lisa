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
//<ID> 997
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong comp_bound = compressBound(ORIG_LEN);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef *readBuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readBuf, 0, (size_t)ORIG_LEN);
    uLong crc_full = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_half = crc32(0L, orig + (ORIG_LEN / 2), (uInt)(ORIG_LEN / 2));

    // step 2: Compress data and write original to a gzip file
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);

    // step 3: Initialize inflate, reset, perform inflate and cleanup inflate structures
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
    inflateEnd(&istrm);

    // step 4: Read back gzip file, compute combined CRC, cleanup
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int bytes_read = gzread(gr, readBuf, (unsigned int)ORIG_LEN);
    gzclose(gr);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc_combined = crc32_combine(crc_full, crc_half, (off_t)(ORIG_LEN / 2));
    free(orig);
    free(comp);
    free(decomp);
    free(readBuf);
    (void)ver;
    (void)bytes_read;
    (void)crc_full;
    (void)crc_half;
    (void)crc_decomp;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}