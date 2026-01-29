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
//<ID> 697
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence test payload for inflateInit2_, deflateCopy, inflateReset and gzgetc_ usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize deflate source stream and make a copy with deflateCopy
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dsrc);

    // step 3: Initialize inflate stream with inflateInit2_, reset it, set input/output and run inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inf_reset = inflateReset(&istrm);
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Write inflated data to a gz file, reopen it for reading and use gzgetc_; then cleanup all resources
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)istrm.total_out);
    int rc_gzclose1 = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int ch = gzgetc_(gf2);
    int rc_gzclose2 = gzclose(gf2);

    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_copy = deflateEnd(&dcopy);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init2;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    (void)ch;
    (void)rc_inf_end;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    // API sequence test completed successfully
    return 66;
}