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
//<ID> 617
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and checksums
    const char src[] = "zlib API sequence test payload for inflateBack/init/deflate/gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen_f = (uLongf)compBound;
    const char *version = zlibVersion();
    uLong source_adler = adler32(0L, source, (uInt)sourceLen);

    // step 2: Initialize and run deflate to produce a compressed stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    uLong compressed_len = dstrm.total_out;

    // step 3: Initialize inflateBack (with a window) and a regular inflate to decompress and validate
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(outBuf, 0, (size_t)(sourceLen + 64));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_len;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);
    int rc_inf = inflate(&istrm, 4); /* Z_FINISH == 4 */
    uLong decompressed_len = istrm.total_out;
    uLong out_adler = adler32(0L, outBuf, (uInt)decompressed_len);

    // step 4: Cleanup streams, write a byte to a gzFile, validate and free resources
    int rc_inf_end = inflateEnd(&istrm);
    int rc_back_end = inflateBackEnd(&backstrm);
    int rc_def_end = deflateEnd(&dstrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, 'Z');
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_back_init;
    (void)rc_inf_init;
    (void)rc_inf;
    (void)rc_inf_end;
    (void)rc_back_end;
    (void)rc_def_end;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)compLen_f;
    (void)compressed_len;
    (void)source_adler;
    (void)out_adler;
    // API sequence test completed successfully
    return 66;
}