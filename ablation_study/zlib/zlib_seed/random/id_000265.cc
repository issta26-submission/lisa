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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> copy -> back-end";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize an inflate stream and perform inflation
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
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Validate inflate sync point, copy the inflate stream state, and initialize an inflate-back stream
    int rc_sync = inflateSyncPoint(&istrm);
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&copyStrm, &istrm);
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(z_stream));
    int rc_back_init = inflateBackInit_(&backStrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Cleanup all zlib resources and allocated memory
    int rc_inf_end = inflateEnd(&istrm);
    int rc_copy_end = inflateEnd(&copyStrm);
    int rc_back_end = inflateBackEnd(&backStrm);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)decompAlloc;
    (void)rc_sync;
    (void)rc_copy;
    (void)rc_back_init;
    (void)rc_inf_end;
    (void)rc_copy_end;
    (void)rc_back_end;
    (void)sourceLen;
    (void)version;

    // API sequence test completed successfully
    return 66;
}