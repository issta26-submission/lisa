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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize source and version
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> validate -> write gz via gzputc";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Compress source into a buffer
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Initialize inflate stream and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf outSize = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outSize);
    memset(outBuf, 0, (size_t)outSize);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outSize;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    int rc_validate = inflateValidate(&istrm, 1);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Write a byte from decompressed output to a .gz file and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_putc = gzputc(gz, (int)(outBuf[0]));
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gz, &gz_errnum);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_validate;
    (void)rc_inflate_end;
    (void)rc_gz_putc;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)rc_gz_close;

    return 66;
    // API sequence test completed successfully
}