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
//<ID> 692
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

    // step 2: Initialize a deflate stream and make a copy of its state
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    z_stream ddst;
    memset(&ddst, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&ddst, &dsrc);

    // step 3: Initialize inflate with inflateInit2_, reset it, and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inf_reset = inflateReset(&istrm);
    uLong outAlloc = sourceLen + 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Write decompressed data to a gzFile, read back a byte with gzgetc_, then cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)istrm.total_out);
    int rc_gzclose1 = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c = gzgetc_(gf2);
    int rc_gzclose2 = gzclose(gf2);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_dst = deflateEnd(&ddst);
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
    (void)c;
    (void)rc_gzclose2;
    (void)rc_inf_end;
    (void)rc_def_end_src;
    (void)rc_def_end_dst;
    // API sequence test completed successfully
    return 66;
}