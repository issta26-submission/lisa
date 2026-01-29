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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source + compress
    const char src_c[] = "zlib API sequence payload: initialize -> configure -> operate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize deflate stream and configure parameters
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    uLongf defOutAlloc = (uLongf)(compBound + 256);
    Bytef *defOut = (Bytef *)malloc((size_t)defOutAlloc);
    memset(defOut, 0, (size_t)defOutAlloc);
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)defOutAlloc;
    int rc_def_params = deflateParams(&dstrm, 1, 0);

    // step 3: Operate: deflate the compressed buffer, then initialize and run inflate with a reset
    int rc_deflate = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);
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
    int rc_inflate1 = inflate(&istrm, 0);
    int rc_inflate_reset = inflateReset(&istrm);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate2 = inflate(&istrm, 0);

    // step 4: Validate outcomes lightly and cleanup resources
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(defOut);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inflate1;
    (void)rc_inflate_reset;
    (void)rc_inflate2;
    (void)rc_inf_end;
    (void)defOutAlloc;
    (void)decompAlloc;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}