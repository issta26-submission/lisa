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
//<ID> 614
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and basic compression parameters
    const char src[] = "zlib API sequence test payload for inflateBackInit_/deflate/gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;

    // step 2: Initialize and run a deflate stream to produce compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    compLen = (uLongf)dstrm.total_out;

    // step 3: Initialize an inflate-back stream and write via gzputc to a gzFile
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_put0 = gzputc(gf, (int)compBuf[0]);
    int rc_gz_put1 = gzputc(gf, (int)src[0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup streams and buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);
    (void)rc_def_init;
    (void)rc_def;
    (void)compLen;
    (void)rc_inf_back_init;
    (void)rc_gz_put0;
    (void)rc_gz_put1;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}