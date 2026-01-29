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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source/compressed buffers
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflateBackInit_ -> deflateSetHeader";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize deflate stream and set a gzip header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 255;
    int rc_set_header = deflateSetHeader(&dstrm, &head);

    // step 3: Initialize inflate-back stream with a window and check sync point
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_sync_point = inflateSyncPoint(&istrm);

    // step 4: Cleanup and finalize
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_back_end = inflateBackEnd(&istrm);
    free(compBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_set_header;
    (void)rc_inf_back_init;
    (void)rc_sync_point;
    (void)rc_def_end;
    (void)rc_inf_back_end;
    (void)version;

    // API sequence test completed successfully
    return 66;
}