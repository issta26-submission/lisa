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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute bounds, and compress source
    const char src_c[] = "zlib API sequence payload: prepare -> inflateBack -> copy -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream, prepare output buffer, and create a copy
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLong decompAlloc = sourceLen + 512;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_infl_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Invoke inflateBack on the copied stream and compute adler32 checksums
    int rc_infback = inflateBack(&istrm_copy, (in_func)0, (void *)0, (out_func)0, (void *)0);
    uLong chk_source = adler32_z(0UL, source, (z_size_t)sourceLen);
    uLong chk_decomp = adler32_z(0UL, decompBuf, (z_size_t)decompAlloc);

    // step 4: Cleanup inflate streams and buffers
    int rc_inf_end1 = inflateEnd(&istrm);
    int rc_inf_end2 = inflateEnd(&istrm_copy);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_infl_copy;
    (void)rc_infback;
    (void)chk_source;
    (void)chk_decomp;
    (void)rc_inf_end1;
    (void)rc_inf_end2;

    // API sequence test completed successfully
    return 66;
}