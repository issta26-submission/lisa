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
//<ID> 698
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence test payload for inflate, deflate copy and gzgetc_ usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and make a copy of it
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    z_stream ddest;
    memset(&ddest, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&ddest, &dsrc);

    // step 3: Initialize inflate with explicit windowBits, reset it, and perform inflate into out buffer
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
    int rc_inf_reset = inflateReset(&istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Write a single byte from the inflated output to a gz file, then read it back using gzgetc_
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gw_put = gzputc(gw, (int)outBuf[0]);
    int rc_gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c = gzgetc_(gr);
    int rc_gr_close = gzclose(gr);

    // Cleanup resources
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_dest = deflateEnd(&ddest);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init2;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)c;
    (void)rc_gw_put;
    (void)rc_gw_close;
    (void)rc_gr_close;
    (void)rc_def_end_src;
    (void)rc_def_end_dest;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}