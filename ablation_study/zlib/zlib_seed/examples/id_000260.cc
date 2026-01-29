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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it
    const char src_c[] = "zlib API sequence payload: prepare -> inflate -> copy -> back-end cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    int rc_sync_point = inflateSyncPoint(&istrm);

    // step 3: Copy the inflate stream state and initialize an inflate-back stream
    z_stream copystrm;
    memset(&copystrm, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&copystrm, &istrm);
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    int rc_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_back_end = inflateBackEnd(&backstrm);

    // step 4: Cleanup all resources and end inflate streams
    int rc_inf_end = inflateEnd(&istrm);
    int rc_copy_end = inflateEnd(&copystrm);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync_point;
    (void)rc_copy;
    (void)rc_back_init;
    (void)rc_back_end;
    (void)rc_inf_end;
    (void)rc_copy_end;
    (void)sourceLen;
    (void)decompAlloc;
    (void)version;

    // API sequence test completed successfully
    return 66;
}