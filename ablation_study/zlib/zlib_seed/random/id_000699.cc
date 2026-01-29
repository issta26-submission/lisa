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
//<ID> 699
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and initialize a deflate template, then copy it
    const char src[] = "zlib API sequence test payload for inflateInit2_, deflateCopy, inflateReset and gzgetc_";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dsrc;
    z_stream dcopy;
    memset(&dsrc, 0, sizeof(z_stream));
    memset(&dcopy, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dsrc);

    // step 2: Compute compression bound from the copied deflate stream and compress using compress2
    uLong compAlloc = deflateBound(&dcopy, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate with inflateInit2_, perform inflate, reset inflate, then write/read via gzFile and gzgetc_
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_reset = inflateReset(&istrm);
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, outBuf, (unsigned int)istrm.total_out);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int ch = gzgetc_(gf_r);
    int rc_gzclose_r = gzclose(gf_r);

    // step 4: Cleanup and validation
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_copy = deflateEnd(&dcopy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_comp;
    (void)rc_inf_init2;
    (void)rc_inflate;
    (void)rc_inf_reset;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)ch;
    (void)rc_gzclose_r;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}