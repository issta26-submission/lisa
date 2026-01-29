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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and version string
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ + inflateSyncPoint + deflateSetHeader usage";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Compress source into a buffer to produce meaningful data flow
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize and configure a deflate stream and set a gzip header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    Bytef *name_buf = (Bytef *)malloc(16);
    memset(name_buf, 0, 16);
    name_buf[0] = 't'; name_buf[1] = 'e'; name_buf[2] = 's'; name_buf[3] = 't'; name_buf[4] = '\0';
    head.name = name_buf;
    head.name_max = 16;
    head.text = 0;
    head.hcrc = 0;
    head.done = 0;
    int rc_set_header = deflateSetHeader(&dstrm, &head);

    // step 4: Initialize inflate-back stream, validate sync point, cleanup both streams and buffers
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infb_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_infb_end = inflateBackEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(name_buf);
    free(window);

    (void)source;
    (void)sourceLen;
    (void)version;
    (void)compBound;
    (void)compLen;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_set_header;
    (void)rc_infb_init;
    (void)rc_sync;
    (void)rc_infb_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}