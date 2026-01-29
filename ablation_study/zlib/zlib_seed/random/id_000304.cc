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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> deflate header -> inflateBack init -> sync -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize and configure a deflate stream and set a gzip header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, sizeof(gz_header));
    hdr.text = 0;
    hdr.time = 0;
    hdr.xflags = 0;
    hdr.os = 255;
    int rc_set_hdr = deflateSetHeader(&dstrm, &hdr);

    // step 3: Initialize an inflate-back stream, attach compressed input, and query sync point
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned int windowSize = 1U << 15;
    unsigned char *window = (unsigned char *)malloc((size_t)windowSize);
    memset(window, 0, (size_t)windowSize);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    int rc_sync_point = inflateSyncPoint(&istrm);

    // step 4: Cleanup streams and buffers
    int rc_inf_back_end = inflateBackEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_set_hdr;
    (void)rc_inf_back_init;
    (void)rc_sync_point;
    (void)rc_inf_back_end;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}