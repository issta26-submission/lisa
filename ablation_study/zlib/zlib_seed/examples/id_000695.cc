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
//<ID> 695
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it
    const char src[] = "zlib API sequence test payload for inflateInit2_, deflateCopy, inflateReset and gzgetc_ usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and create a copy with deflateCopy, then clean them up
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dsrc);
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_copy = deflateEnd(&dcopy);

    // step 3: Initialize inflate stream with inflateInit2_, reset it, perform inflate into an output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLong outAlloc = sourceLen + 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inf_reset = inflateReset(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    outBuf[(size_t)istrm.total_out] = 0; // null-terminate decompressed data

    // step 4: Write decompressed data to a gzFile, reopen it and read one character with gzgetc_, then cleanup
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)outBuf, (unsigned int)istrm.total_out);
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int single_char = gzgetc_(gr);
    int rc_gzclose_r = gzclose(gr);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inf_init2;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)single_char;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}