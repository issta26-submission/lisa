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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare and compress source data
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> inflateCopy -> inflateBackEnd";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and decompress
    z_stream src;
    memset(&src, 0, (int)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&src, version, (int)sizeof(z_stream));
    src.next_in = compBuf;
    src.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    src.next_out = decompBuf;
    src.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&src, 0);
    uLong actual_out = src.total_out;

    // step 3: Copy inflate state and validate sync point
    z_stream copy;
    memset(&copy, 0, (int)sizeof(z_stream));
    int rc_copy = inflateCopy(&copy, &src);
    int rc_sync = inflateSyncPoint(&copy);

    // step 4: Initialize inflate-back stream for cleanup test and then cleanup all streams and buffers
    z_stream backstrm;
    memset(&backstrm, 0, (int)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    int rc_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_back_end = inflateBackEnd(&backstrm);
    int rc_end_src = inflateEnd(&src);
    int rc_end_copy = inflateEnd(&copy);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)rc_copy;
    (void)rc_sync;
    (void)rc_back_init;
    (void)rc_back_end;
    (void)rc_end_src;
    (void)rc_end_copy;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}