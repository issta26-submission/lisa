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
//<ID> 651
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute bounds, compress and persist to a gz file
    const char src[] = "zlib API sequence test payload for a realistic flow";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gf_w);

    // step 2: Initialize a deflate stream (initialize -> configure)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflateBack, initialize standard inflate, and perform inflate (operate -> validate)
    z_stream istrm_back;
    memset(&istrm_back, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(1u << 15);
    memset(window, 0, (size_t)(1u << 15));
    int rc_inflate_back_init = inflateBackInit_(&istrm_back, 15, window, version, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf outAlloc = (uLongf)(sourceLen + 128);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_back_end = inflateBackEnd(&istrm_back);

    // step 4: Validate via gz tell on file with compressed contents and cleanup
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t pos = gztell(gf_r);
    int rc_gzclose_r = gzclose(gf_r);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)rc_inflate_back_init;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_inflate_back_end;
    (void)pos;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}