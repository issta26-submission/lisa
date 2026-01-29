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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize — prepare source, compress into zlib format
    const char src_c[] = "zlib API sequence payload: compress then inflate, then write a byte to gz";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Configure — prepare inflate stream and output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;

    // step 3: Operate — inflate the compressed data
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate & Cleanup — validate inflate state, end inflate, write to gz and cleanup
    int rc_validate = inflateValidate(&istrm, 1);
    int rc_inf_end = inflateEnd(&istrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_putc = gzputc(gz, 'X');
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
    (void)errstr;
    (void)errnum;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}