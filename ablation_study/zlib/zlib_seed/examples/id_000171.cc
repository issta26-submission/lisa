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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize source and compression buffers
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> gzputc -> gzerror";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);

    // step 2: Setup inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompLen;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate & Validate inflate, then cleanup inflate
    int rc_inflate = inflate(&istrm, 0);
    int rc_validate = inflateValidate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write a byte to a gzip file, query error string, cleanup buffers and close
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_putc = gzputc(gz, 'Z');
    int errnum = 0;
    const char *errstr = gzerror(gz, &errnum);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(decompBuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_validate;
    (void)rc_inf_end;
    (void)rc_putc;
    (void)errnum;
    (void)errstr;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}