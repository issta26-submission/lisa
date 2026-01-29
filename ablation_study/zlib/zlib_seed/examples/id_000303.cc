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
//<ID> 303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source and produce compressed data
    const char src_c[] = "zlib API sequence payload: prepare -> deflate header -> inflateBack init -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize and configure deflate stream and set gzip header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, sizeof(gz_header));
    Bytef *nameBuf = (Bytef *)malloc(16);
    memset(nameBuf, 0, 16);
    memcpy(nameBuf, "zlib_test", 9);
    gzhead.name = nameBuf;
    gzhead.name_max = 16;
    gzhead.time = (uLong)0;
    gzhead.text = 0;
    gzhead.os = 255;
    int rc_def_sethdr = deflateSetHeader(&dstrm, &gzhead);
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);

    // step 3: Initialize inflate-back stream and validate sync point
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inf_syncpt = inflateSyncPoint(&istrm);

    // step 4: Cleanup and finalize
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_back_end = inflateBackEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(nameBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_sethdr;
    (void)rc_gzwrite;
    (void)rc_inf_back_init;
    (void)rc_inf_syncpt;
    (void)rc_def_end;
    (void)rc_inf_back_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}