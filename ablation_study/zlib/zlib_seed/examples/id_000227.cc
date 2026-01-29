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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it with compress()
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> inflate -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLongf compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Uncompress back to a buffer using uncompress(), and initialize inflate stream
    uLongf decompBufLen = (uLongf)(sourceLen + 128);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompBufLen);
    memset(decompBuf, 0, (size_t)decompBufLen);
    uLongf actualDecompLen = decompBufLen;
    int rc_uncomp = uncompress(decompBuf, &actualDecompLen, compBuf, (uLong)compLen);

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *ver = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Use inflate() on the compressed buffer into a fresh output buffer, write to gz, rewind, and write again
    Bytef *inflateOut = (Bytef *)malloc((size_t)(sourceLen + 256));
    memset(inflateOut, 0, (size_t)(sourceLen + 256));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = inflateOut;
    istrm.avail_out = (uInt)(sourceLen + 256);
    int rc_inflate = inflate(&istrm, 0);
    uLong inflatedBytes = istrm.total_out;

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write1 = gzwrite(gz, (voidpc)decompBuf, (unsigned int)actualDecompLen);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_write2 = gzwrite(gz, (voidpc)inflateOut, (unsigned int)inflatedBytes);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup inflate stream and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);
    free(inflateOut);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflatedBytes;
    (void)rc_gz_write1;
    (void)rc_gz_rewind;
    (void)rc_gz_write2;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)compBound;
    (void)actualDecompLen;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}