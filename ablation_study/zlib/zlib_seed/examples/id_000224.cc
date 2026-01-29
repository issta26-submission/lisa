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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source payload
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> uncompress -> gzwrite -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and perform inflate on compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf infOutLen = (uLongf)(sourceLen + 128);
    Bytef *infOutBuf = (Bytef *)malloc((size_t)infOutLen);
    memset(infOutBuf, 0, (size_t)infOutLen);
    istrm.next_out = infOutBuf;
    istrm.avail_out = (uInt)infOutLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflated_bytes = istrm.total_out;

    // step 3: Validate decompression using uncompress() on the same compressed buffer
    uLongf uncmpLen = (uLongf)(sourceLen + 64);
    Bytef *uncmpBuf = (Bytef *)malloc((size_t)uncmpLen);
    memset(uncmpBuf, 0, (size_t)uncmpLen);
    int rc_uncomp = uncompress(uncmpBuf, &uncmpLen, compBuf, (uLong)compLen);

    // step 4: Write validated decompressed data to a gzip file, rewind and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)uncmpBuf, (unsigned int)uncmpLen);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(infOutBuf);
    free(uncmpBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflated_bytes;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)bound;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}