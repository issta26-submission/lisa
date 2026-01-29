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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    const char src_c[] = "zlib payload for inflate -> gzputc demo";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;

    // step 2: Compress to produce a zlib-wrapped stream
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate stream and perform a single inflate call
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen + 32;
    Bytef *outBuf = (Bytef *)malloc((size_t)outCap);
    memset(outBuf, 0, (size_t)outCap);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outCap;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_validate = inflateValidate(&istrm, 0);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Write a couple of decompressed bytes to a gz file, retrieve error string, and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_putc1 = gzputc(gz, (int)outBuf[0]);
    int rc_gz_putc2 = gzputc(gz, (int)outBuf[1]);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gz, &gz_errnum);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_validate;
    (void)rc_inflate;
    (void)rc_gz_putc1;
    (void)rc_gz_putc2;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_gz_close;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}