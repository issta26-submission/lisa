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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize source and compression buffers
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> write gz via gzputc";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf outLen = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outLen);
    memset(outBuf, 0, (size_t)outLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outLen;

    // step 3: Validate stream, perform inflate, and record results
    int rc_validate = inflateValidate(&istrm, 0);
    int rc_inflate = inflate(&istrm, 0);
    uLong decompressed_used = istrm.total_out;

    // step 4: Write a byte to gz, query error, cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_putc = gzputc(gz, decompressed_used > 0 ? (int)outBuf[0] : (int)'\n');
    int gz_errnum = 0;
    const char * gz_errstr = gzerror(gz, &gz_errnum);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_validate;
    (void)rc_inflate;
    (void)decompressed_used;
    (void)rc_gz_putc;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_gz_close;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}