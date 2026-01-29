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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> write gz -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;

    // step 2: Setup & Compress
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Configure & Operate (inflate)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompSize = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompSize);
    memset(decompBuf, 0, (size_t)decompSize);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompSize;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inf_validate = inflateValidate(&istrm, 1);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Operate (write gzip) & Cleanup/Validate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_putc = gzputc(gz, (int)decompBuf[0]);
    int gz_errnum = 0;
    const char * gz_errstr = gzerror(gz, &gz_errnum);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_validate;
    (void)rc_inflate;
    (void)rc_gz_putc;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)rc_gz_close;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}