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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source data
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> inflate -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Uncompress to a buffer using uncompress for validation
    uLongf decomp1Len = (uLongf)(sourceLen + 64);
    Bytef *decomp1 = (Bytef *)malloc((size_t)decomp1Len);
    memset(decomp1, 0, (size_t)decomp1Len);
    int rc_uncomp = uncompress(decomp1, &decomp1Len, compBuf, (uLong)compLen);

    // step 3: Initialize inflate stream and perform inflate on compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decomp2Len = (uLongf)(sourceLen + 128);
    Bytef *decomp2 = (Bytef *)malloc((size_t)decomp2Len);
    memset(decomp2, 0, (size_t)decomp2Len);
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)decomp2Len;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write decompressed data to a gzip file, rewind it, write again, and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write1 = gzwrite(gz, (voidpc)decomp1, (unsigned int)decomp1Len);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_write2 = gzwrite(gz, (voidpc)decomp2, (unsigned int)actual_out);
    int rc_gz_close = gzclose(gz);

    free(compBuf);
    free(decomp1);
    free(decomp2);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gz_write1;
    (void)rc_gz_rewind;
    (void)rc_gz_write2;
    (void)rc_gz_close;
    (void)bound;
    (void)sourceLen;
    (void)actual_out;

    // API sequence test completed successfully
    return 66;
}