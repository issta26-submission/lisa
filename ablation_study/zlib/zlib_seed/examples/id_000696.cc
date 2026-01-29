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
//<ID> 696
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compress into a buffer, and allocate output buffer
    const char src[] = "zlib API sequence payload for testing inflateInit2_, deflateCopy, inflateReset, and gzgetc_";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong outAlloc = sourceLen + 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);

    // step 2: Initialize a deflate source stream and copy it into a destination stream
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dsrc);

    // step 3: Initialize inflate stream with inflateInit2_, reset it, and perform inflate into outBuf
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inf_reset = inflateReset(&istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Write inflated output to a gz file, reopen for reading, use gzgetc_, then cleanup
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, outBuf, (unsigned int)istrm.total_out);
    int rc_gwclose = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int ch = gzgetc_(gr);
    int rc_grclose = gzclose(gr);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end_copy = deflateEnd(&dcopy);
    int rc_def_end_src = deflateEnd(&dsrc);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init2;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gwclose;
    (void)ch;
    (void)rc_grclose;
    (void)rc_inf_end;
    (void)rc_def_end_copy;
    (void)rc_def_end_src;
    // API sequence test completed successfully
    return 66;
}