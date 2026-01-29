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
//<ID> 1000
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
    Bytef *inflated = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(inflated, 0, (size_t)ORIG_LEN);
    Bytef *uncomp_out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(uncomp_out, 0, (size_t)ORIG_LEN);

    // step 2: Compress data and exercise gzFile + gzclearerr
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gw);
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);

    // step 3: Initialize inflate stream, prime bits, inflate compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = inflated;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 1, 1);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Use uncompress2 to validate decompression, clear error on read gzFile, cleanup
    uLongf destLen = (uLongf)ORIG_LEN;
    uLong comp_source_len = (uLong)comp_len;
    uncompress2(uncomp_out, &destLen, comp, &comp_source_len);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclearerr(gr);
    gzclose(gr);
    free(orig);
    free(comp);
    free(inflated);
    free(uncomp_out);
    (void)ver;
    (void)destLen;
    (void)comp_source_len;
    // API sequence test completed successfully
    return 66;
}