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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> inflate -> copy -> inflateBackEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream, perform inflate, check sync point and copy the stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 128);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    int rc_sync = inflateSyncPoint(&istrm);
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Initialize an inflate-back stream (with window) and then end it
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    int rc_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_back_end = inflateBackEnd(&bstrm);

    // step 4: Cleanup inflate streams and allocated memory
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);
    free(window);
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync;
    (void)rc_copy;
    (void)rc_back_init;
    (void)rc_back_end;
    (void)rc_inf_end_copy;
    (void)rc_inf_end;
    (void)sourceLen;
    (void)version;
    // API sequence test completed successfully
    return 66;
}