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
//<ID> 654
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers, and perform deflate
    const char src[] = "zlib API sequence test payload for compress/inflate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 4);
    int rc_def_end = deflateEnd(&dstrm);

    // step 2: Persist compressed bytes to a gzip file and query position
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)dstrm.total_out);
    off_t pos = gztell(gf);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize inflate and decompress the deflate output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 4);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Initialize inflateBack (window buffer provided), validate and cleanup
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    int rc_inflateBackInit = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflateBackEnd = inflateBackEnd(&bstrm);

    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_inflateBackInit;
    (void)rc_inflateBackEnd;
    // API sequence test completed successfully
    return 66;
}