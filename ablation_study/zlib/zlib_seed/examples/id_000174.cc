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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize source and compression buffers
    const char src_c[] = "zlib API sequence payload: compress then inflate then write bytes to gz via gzputc";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    const char *version = zlibVersion();

    // step 2: Setup inflate stream and output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 32);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Validate inflate state, finish inflate, and write some output bytes to gz
    int rc_validate = inflateValidate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gc1 = gzputc(gz, (int)decompBuf[0]);
    int gc2 = gzputc(gz, (int)decompBuf[1]);
    int gc3 = gzputc(gz, (int)decompBuf[2]);
    int errnum = 0;
    const char *gz_err = gzerror(gz, &errnum);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    free(compBuf);
    free(decompBuf);
    (void)rc_comp;
    (void)version;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_validate;
    (void)rc_inf_end;
    (void)gc1;
    (void)gc2;
    (void)gc3;
    (void)gz_err;
    (void)errnum;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}