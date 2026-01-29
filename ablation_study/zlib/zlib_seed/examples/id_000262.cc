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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: prepare -> inflate -> copy -> back-end cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and perform an inflate operation
    z_stream baseStrm;
    memset(&baseStrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&baseStrm, version, (int)sizeof(z_stream));
    baseStrm.next_in = compBuf;
    baseStrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 128);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    baseStrm.next_out = decompBuf;
    baseStrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&baseStrm, 0);
    uLong decompressed_bytes = baseStrm.total_out;

    // step 3: Copy the inflate stream, query sync point, and initialize an inflate-back stream then end it
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&copyStrm, &baseStrm);
    int rc_sync = inflateSyncPoint(&copyStrm);
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(z_stream));
    int rc_back_init = inflateBackInit_(&backStrm, 15, window, version, (int)sizeof(z_stream));
    int rc_back_end = inflateBackEnd(&backStrm);

    // step 4: Cleanup inflate streams and allocated buffers
    int rc_copy_end = inflateEnd(&copyStrm);
    int rc_base_end = inflateEnd(&baseStrm);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)decompressed_bytes;
    (void)rc_copy;
    (void)rc_sync;
    (void)rc_back_init;
    (void)rc_back_end;
    (void)rc_copy_end;
    (void)rc_base_end;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}