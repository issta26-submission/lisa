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
//<ID> 616
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and initialize deflate stream and buffers
    const char src[] = "payload for zlib deflate/inflateBack/gzputc test";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */

    // step 2: Initialize inflate-back stream (inflateBackInit_) and then finalize it with inflateEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(1);
    memset(window, 0, 1);
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);
    free(window);

    // step 3: Open a gzFile, write a single byte with gzputc, and close the file
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, 'Z');
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream and buffers
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_infback_init;
    (void)rc_inflate_end;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}