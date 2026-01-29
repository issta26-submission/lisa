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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source into compBuf
    const char src_c[] = "zlib API sequence payload: compress -> inflateInit_ -> inflate -> uncompress -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and perform inflate on compBuf
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompBufLen = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompBufLen);
    memset(decompBuf, 0, (size_t)decompBufLen);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompBufLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflated_bytes = istrm.total_out;

    // step 3: Use uncompress on same compressed data, write results to gzip, rewind and write again
    uLongf decomp2Len = (uLongf)(sourceLen + 128);
    Bytef *decomp2 = (Bytef *)malloc((size_t)decomp2Len);
    memset(decomp2, 0, (size_t)decomp2Len);
    int rc_uncomp = uncompress(decomp2, &decomp2Len, compBuf, compLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write1 = gzwrite(gz, (voidpc)decomp2, (unsigned int)decomp2Len);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_write2 = gzwrite(gz, (voidpc)decompBuf, (unsigned int)inflated_bytes);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup inflate stream and free allocations
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);
    free(decomp2);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflated_bytes;
    (void)rc_uncomp;
    (void)rc_gz_write1;
    (void)rc_gz_rewind;
    (void)rc_gz_write2;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)bound;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}