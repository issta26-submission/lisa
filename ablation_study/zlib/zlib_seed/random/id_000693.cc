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
//<ID> 693
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence payload for inflateInit2_, deflateCopy, inflateReset and gzgetc_ usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and copy it
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Initialize inflate stream with inflateInit2_, reset it, inflate the compressed buffer, and write output to a gz file
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
    gzFile gf_write = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_write, outBuf, (unsigned int)istrm.total_out);
    int rc_gzclose_write = gzclose(gf_write);
    gzFile gf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int ch = gzgetc_(gf_read);
    int rc_gzclose_read = gzclose(gf_read);

    // step 4: Validate and cleanup resources
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end_orig = deflateEnd(&dstrm);
    int rc_def_end_copy = deflateEnd(&dstrm_copy);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init2;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose_write;
    (void)ch;
    (void)rc_gzclose_read;
    (void)rc_inf_end;
    (void)rc_def_end_orig;
    (void)rc_def_end_copy;
    return 66;
    // API sequence test completed successfully
}