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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source/compressed buffers
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflateBackInit -> header";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize and configure deflate stream with gzip header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15 + 16, 8, 0, version, (int)sizeof(z_stream));
    gz_header header;
    header.text = 0;
    header.time = 0;
    header.xflags = 0;
    header.os = 255;
    header.extra = NULL;
    header.extra_len = 0;
    header.extra_max = 0;
    header.name = (Bytef *)"zlib_test_name";
    header.name_max = (uInt)(sizeof("zlib_test_name") - 1);
    header.comment = NULL;
    header.comm_max = 0;
    header.hcrc = 0;
    header.done = 0;
    int rc_def_sethdr = deflateSetHeader(&dstrm, &header);

    // step 3: Initialize inflate-back stream and prepare output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(1U << 15);
    memset(window, 0, (size_t)(1U << 15));
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;

    // step 4: Validate state, teardown and cleanup
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_infback_end = inflateBackEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)source;
    (void)sourceLen;
    (void)version;
    (void)compBound;
    (void)compLen;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_sethdr;
    (void)rc_infback_init;
    (void)rc_sync;
    (void)rc_infback_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}