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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> inflateValidate -> gzwrite via gzputc";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup & Compress, initialize inflate stream
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef *decompOut = (Bytef *)malloc((size_t)(sourceLen + 32));
    memset(decompOut, 0, (size_t)(sourceLen + 32));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompOut;
    istrm.avail_out = (uInt)(sourceLen + 32);

    // step 3: Operate (write to gz using gzputc) & validate inflate stream and query gz error
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_putc1 = gzputc(gz, (int)src_c[0]);
    int rc_gz_putc2 = gzputc(gz, (int)src_c[1]);
    int rc_gz_putc3 = gzputc(gz, (int)src_c[2]);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gz, &gz_errnum);
    int rc_inf_val = inflateValidate(&istrm, 0);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup & finalization
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompOut);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_gz_putc1;
    (void)rc_gz_putc2;
    (void)rc_gz_putc3;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_inf_val;
    (void)rc_gz_close;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}