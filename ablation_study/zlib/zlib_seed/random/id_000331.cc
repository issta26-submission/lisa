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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and perform a high-level compress
    const char src_c[] = "zlib API sequence payload: initialize -> configure -> operate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream, configure parameters, but do not perform full streaming deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    uLongf dummyOutSize = (uLongf)(compLen + 128);
    Bytef *dummyOut = (Bytef *)malloc((size_t)dummyOutSize);
    memset(dummyOut, 0, (size_t)dummyOutSize);
    dstrm.next_out = dummyOut;
    dstrm.avail_out = (uInt)dummyOutSize;
    int rc_def_params = deflateParams(&dstrm, 9, 1);

    // step 3: Initialize an inflate stream, reset it, set compressed input and perform inflate to validate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate_reset = inflateReset(&istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Cleanup streams and buffers
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(dummyOut);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_inf_init;
    (void)rc_inflate_reset;
    (void)rc_inflate;
    (void)rc_def_end;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}