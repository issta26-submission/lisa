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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source into a zlib buffer
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> inflate -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Uncompress the compressed data into a separate buffer
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    // step 3: Initialize inflate stream and perform inflate on the compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf inf_out_len = (uLongf)(sourceLen + 128);
    Bytef *infOut = (Bytef *)malloc((size_t)inf_out_len);
    memset(infOut, 0, (size_t)inf_out_len);
    istrm.next_out = infOut;
    istrm.avail_out = (uInt)inf_out_len;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write inflated output to a gzip file, rewind it, close and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)infOut, (unsigned int)actual_out);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(decompBuf);
    free(infOut);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)bound;
    (void)sourceLen;
    (void)actual_out;

    // API sequence test completed successfully
    return 66;
}