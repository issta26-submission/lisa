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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // step 1: Prepare source, compress into buffer
    const char src_c[] = "zlib API sequence payload: inflateBack + inflateCopy + adler32_z usage";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize an inflateBack-capable stream and allocate window + output buffer
    z_stream iback_strm;
    memset(&iback_strm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_iback_init = inflateBackInit_(&iback_strm, 15, window, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    iback_strm.next_in = compBuf;
    iback_strm.avail_in = (uInt)compLen;
    iback_strm.next_out = decompBuf;
    iback_strm.avail_out = (uInt)decompAlloc;

    // step 3: Initialize a regular inflate stream, copy it, compute adler32_z, and invoke inflateBack
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc2 = (uLongf)(sourceLen + 512);
    Bytef *decompBuf2 = (Bytef *)malloc((size_t)decompAlloc2);
    memset(decompBuf2, 0, (size_t)decompAlloc2);
    istrm.next_out = decompBuf2;
    istrm.avail_out = (uInt)decompAlloc2;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate_orig = inflate(&istrm, 0);
    int rc_inflate_copy_run = inflate(&istrm_copy, 0);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    int rc_infback = inflateBack(&iback_strm, (in_func)NULL, NULL, (out_func)NULL, NULL);

    // step 4: Cleanup streams and buffers
    int rc_inf_end_iback = inflateEnd(&iback_strm);
    int rc_inf_end_orig = inflateEnd(&istrm);
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    free(compBuf);
    free(decompBuf);
    free(decompBuf2);
    free(window);

    (void)version;
    (void)compBound;
    (void)rc_comp;
    (void)compLen;
    (void)rc_iback_init;
    (void)decompAlloc;
    (void)decompAlloc2;
    (void)rc_inf_init;
    (void)rc_inflate_copy;
    (void)rc_inflate_orig;
    (void)rc_inflate_copy_run;
    (void)checksum;
    (void)rc_infback;
    (void)rc_inf_end_iback;
    (void)rc_inf_end_orig;
    (void)rc_inf_end_copy;

    // API sequence test completed successfully
    return 66;
}