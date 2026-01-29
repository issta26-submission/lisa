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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress it into a buffer
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> set header -> inflateBackInit";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize deflate stream, set a gz_header, and attach it to the deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header gh;
    memset(&gh, 0, sizeof(gz_header));
    Bytef *nameBuf = (Bytef *)malloc(16);
    memset(nameBuf, 0, 16);
    Bytef *commentBuf = (Bytef *)malloc(24);
    memset(commentBuf, 0, 24);
    memcpy(nameBuf, "zlib_test", 9);
    memcpy(commentBuf, "deflate header attached", 22);
    gh.name = nameBuf;
    gh.name_max = 16;
    gh.comment = commentBuf;
    gh.comm_max = 24;
    gh.extra = NULL;
    gh.extra_len = gh.extra_max = 0;
    gh.text = 0;
    gh.time = 0;
    gh.xflags = 0;
    gh.os = 255;
    gh.hcrc = 0;
    gh.done = 0;
    int rc_set_header = deflateSetHeader(&dstrm, &gh);

    // step 3: Initialize inflate-back stream with a sliding window and validate sync point
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << windowBits));
    memset(window, 0, (size_t)(1 << windowBits));
    int rc_inflate_back_init = inflateBackInit_(&istrm, windowBits, window, version, (int)sizeof(z_stream));
    int rc_sync_point = inflateSyncPoint(&istrm);

    // step 4: Cleanup resources and finalize streams
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    free(compBuf);
    free(nameBuf);
    free(commentBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_set_header;
    (void)rc_inflate_back_init;
    (void)rc_sync_point;
    (void)rc_def_end;
    (void)rc_inflate_back_end;
    (void)version;

    // API sequence test completed successfully
    return 66;
}