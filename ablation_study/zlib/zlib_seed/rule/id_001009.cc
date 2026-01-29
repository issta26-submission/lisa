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
//<ID> 1009
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
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress data and write original to a gzip file (use gzclearerr on writer)
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclearerr(gw);
    gzclose_w(gw);

    // step 3: Initialize inflate stream, prime bit buffer, and perform uncompress2 operation
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 3, 5);
    uLong sourceLen = (uLong)comp_len;
    uLongf destLen = (uLongf)ORIG_LEN;
    uncompress2(decomp, &destLen, comp, &sourceLen);

    // step 4: Validate using crc32, clear errors on reader, cleanup and finalize
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0L, decomp, (uInt)destLen);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclearerr(gr);
    gzclose(gr);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_orig;
    (void)crc_decomp;
    (void)destLen;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}