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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflateBackInit_ -> deflate header";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize an inflate-back stream with a sliding window and probe sync point
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int windowBits = 15;
    size_t windowSize = (size_t)1u << windowBits;
    unsigned char *window = (unsigned char *)malloc(windowSize);
    memset(window, 0, windowSize);
    int rc_inf_back_init = inflateBackInit_(&ibstrm, windowBits, window, version, (int)sizeof(z_stream));
    int rc_sync_point = inflateSyncPoint(&ibstrm);
    int rc_inf_back_end = inflateBackEnd(&ibstrm);

    // step 3: Initialize a deflate stream, set a gzip header, then end the deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    const char hdr_name_c[] = "zlib_api_name";
    Bytef *hdr_name = (Bytef *)malloc((size_t)sizeof(hdr_name_c));
    memcpy(hdr_name, hdr_name_c, (size_t)sizeof(hdr_name_c));
    head.name = hdr_name;
    head.name_max = (uInt)sizeof(hdr_name_c);
    head.comment = NULL;
    head.extra = NULL;
    head.hcrc = 0;
    head.done = 0;
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 0;
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Cleanup allocated buffers and silence unused warnings
    free(compBuf);
    free(window);
    free(hdr_name);
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_back_init;
    (void)rc_sync_point;
    (void)rc_inf_back_end;
    (void)rc_def_init;
    (void)rc_set_header;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}