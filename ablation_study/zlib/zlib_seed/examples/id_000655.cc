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
//<ID> 655
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and allocate buffers using compressBound
    const char src[] = "zlib API sequence test payload for deflate/inflate and gz operations";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLong compLen = 0;

    // step 2: Initialize and run a deflate stream to produce compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 4); // 4 == Z_FINISH
    compLen = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist compressed data to a gzFile and query current position with gztell
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    off_t gz_pos = gztell(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflateBack (aux) and a regular inflate stream, then inflate back to output buffer and cleanup
    uLong outAlloc = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 4); // 4 == Z_FINISH
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_infback_end = inflateBackEnd(&bstrm);

    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)gz_pos;
    (void)rc_gzclose;
    (void)rc_infback_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_infback_end;
    // API sequence test completed successfully
    return 66;
}