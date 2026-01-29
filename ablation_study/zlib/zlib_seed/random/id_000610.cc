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
//<ID> 610
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence test payload for deflate/inflateBack/gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    uInt compBufSize = (uInt)(compBound + 16);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);

    // step 2: Initialize and run deflate (Initialize → Configure → Operate)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */

    // step 3: Initialize inflate for back-inflation context and write a byte to a gzFile (Operate → Validate)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, (int)src[0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup (Cleanup → Validate)
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_inf_back_init;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}