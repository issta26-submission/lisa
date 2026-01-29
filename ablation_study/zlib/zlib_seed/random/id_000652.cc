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
//<ID> 652
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and version
    const char src[] = "zlib API sequence test payload for deflate/inflate and gz operations";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLong outAlloc = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);

    // step 2: Initialize and run deflate stream to produce compressed bytes
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 4);
    uLong compProduced = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist compressed bytes to a gz file, reopen and query position
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compProduced);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t pos = gztell(gf_r);
    int rc_gzclose_r = gzclose(gf_r);

    // step 4: Initialize inflate stream, perform inflate, initialize/teardown inflateBack
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 4);
    int rc_inf_end = inflateEnd(&istrm);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&bstrm);

    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)pos;
    (void)rc_gzclose_r;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}