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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> header/init -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize and configure deflate stream, set gzip header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 1;
    head.time = 0;
    head.xflags = 0;
    head.os = 255;
    int rc_def_sethdr = deflateSetHeader(&dstrm, &head);

    // step 3: Initialize inflateBack stream with a sliding window and validate sync point
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    unsigned char *window = (unsigned char *)malloc((size_t)(1u << windowBits));
    memset(window, 0, (size_t)(1u << windowBits));
    int rc_infback_init = inflateBackInit_(&istrm, windowBits, window, version, (int)sizeof(z_stream));
    int rc_sync_point = inflateSyncPoint(&istrm);

    // step 4: Operate (write compressed data to a gz file) and cleanup resources
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 2);
    int rc_gzclose = gzclose(gzf);
    int rc_infback_end = inflateBackEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_sethdr;
    (void)rc_infback_init;
    (void)rc_sync_point;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_infback_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}