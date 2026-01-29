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
//<ID> 339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence payload: compress -> inflate -> deflateParams -> reset -> inflate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and perform first inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate1 = inflate(&istrm, 0);

    // step 3: Initialize deflate stream, adjust parameters, and end deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 1, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Reset inflate stream, perform a second inflate, then cleanup
    int rc_inf_reset = inflateReset(&istrm);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc2 = (uLongf)(sourceLen + 128);
    Bytef *decompBuf2 = (Bytef *)malloc((size_t)decompAlloc2);
    memset(decompBuf2, 0, (size_t)decompAlloc2);
    istrm.next_out = decompBuf2;
    istrm.avail_out = (uInt)decompAlloc2;
    int rc_inflate2 = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    free(decompBuf);
    free(decompBuf2);

    (void)version;
    (void)compBound;
    (void)compLen;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate1;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_def_end;
    (void)rc_inf_reset;
    (void)rc_inflate2;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}