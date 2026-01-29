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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src_c[] = "zlib API sequence payload: prepare -> inflate -> copy -> sync -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and create a copy of its state
    z_stream src;
    memset(&src, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_init = inflateInit_(&src, version, (int)sizeof(z_stream));
    src.next_in = compBuf;
    src.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 128);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    src.next_out = decompBuf;
    src.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&src, 0);
    z_stream copy;
    memset(&copy, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&copy, &src);

    // step 3: Validate sync point on the copied stream and perform inflate-back cleanup
    int rc_sync = inflateSyncPoint(&copy);
    int rc_backend = inflateBackEnd(&copy);
    int rc_copy_end = inflateEnd(&copy);

    // step 4: Cleanup original stream and buffers
    int rc_src_end = inflateEnd(&src);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_init;
    (void)rc_inflate;
    (void)rc_copy;
    (void)rc_sync;
    (void)rc_backend;
    (void)rc_copy_end;
    (void)rc_src_end;
    (void)sourceLen;
    (void)version;
    // API sequence test completed successfully
    return 66;
}