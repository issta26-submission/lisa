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
//<ID> 611
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers
    const char src[] = "zlib API sequence test payload for deflate/inflateBack/gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Initialize and run deflate, then write first compressed byte to a gzFile
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, (int)(compBuf[0] & 0xFF));

    // step 3: Initialize inflateBack for validation (no callbacks used here) and immediately end it
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Cleanup deflate stream, close gzFile, free buffers
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    free(window);

    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzputc;
    (void)rc_inf_back_init;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}