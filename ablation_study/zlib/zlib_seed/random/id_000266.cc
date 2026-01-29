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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare and compress source data
    const char src[] = "zlib API sequence payload: compress -> inflate -> copy -> sync -> back-end";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream, a copy target, and an inflate-back stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    z_stream copystrm;
    memset(&copystrm, 0, sizeof(z_stream));

    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    int rc_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Inflate compressed data, copy the inflate state, check sync points, and teardown back-end
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);

    int rc_copy = inflateCopy(&copystrm, &istrm);
    int sync_orig = inflateSyncPoint(&istrm);
    int sync_copy = inflateSyncPoint(&copystrm);

    int rc_back_end = inflateBackEnd(&bstrm);

    // step 4: Cleanup inflate streams and allocated buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_inf_end_copy = inflateEnd(&copystrm);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)rc_comp;
    (void)compLen;
    (void)bound;
    (void)rc_inf_init;
    (void)rc_back_init;
    (void)rc_inflate;
    (void)rc_copy;
    (void)sync_orig;
    (void)sync_copy;
    (void)rc_back_end;
    (void)rc_inf_end;
    (void)rc_inf_end_copy;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}