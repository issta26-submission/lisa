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
//<ID> 269
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

    // step 2: Initialize inflate streams (normal inflate and inflate-back) and prepare buffers
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    z_stream copy;
    memset(&copy, 0, sizeof(z_stream));
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    int rc_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate: inflate compressed data, check sync point, and make a copy of the inflater state
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_copy_init = inflateInit_(&copy, version, (int)sizeof(z_stream));
    int rc_copy = inflateCopy(&copy, &istrm);

    // step 4: Cleanup streams and memory
    int rc_inf_end = inflateEnd(&istrm);
    int rc_copy_end = inflateEnd(&copy);
    int rc_back_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_back_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync;
    (void)rc_copy_init;
    (void)rc_copy;
    (void)rc_inf_end;
    (void)rc_copy_end;
    (void)rc_back_end;
    (void)version;
    (void)sourceLen;
    (void)decompAlloc;

    // API sequence test completed successfully
    return 66;
}