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
//<ID> 615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and version
    const char src[] = "zlib API sequence test payload for inflateBack/init/end/gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong outBound = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    const char *version = zlibVersion();

    // step 2: Initialize deflate, compress source, and cleanup deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    uLong compressedLen = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate, decompress compressed data, end inflate, and write a byte to gzFile
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressedLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_inf = inflate(&istrm, 4); /* Z_FINISH == 4 */
    int rc_inf_end = inflateEnd(&istrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, (int)((outBuf[0] != 0) ? outBuf[0] : 'X'));
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflateBack with a window buffer, then cleanup remaining resources
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    unsigned char *windowBuf = (unsigned char *)malloc(32768);
    memset(windowBuf, 0, 32768);
    int rc_ib_init = inflateBackInit_(&ibstrm, 15, windowBuf, version, (int)sizeof(z_stream));
    int rc_ib_end = inflateBackEnd(&ibstrm);
    free(windowBuf);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inf;
    (void)rc_inf_end;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_ib_init;
    (void)rc_ib_end;
    // API sequence test completed successfully
    return 66;
}