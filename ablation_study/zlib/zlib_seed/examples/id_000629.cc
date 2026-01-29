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
//<ID> 629
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers
    const char src[] = "zlib API sequence test payload for combined APIs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufAlloc = (uLongf)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufAlloc);
    memset(compBuf, 0, (size_t)compBufAlloc);
    uLongf compDestLen = compBufAlloc;

    // step 2: One-shot compression using compress()
    int rc_compress = compress(compBuf, &compDestLen, source, sourceLen);

    // step 3: Initialize and copy a deflate stream, initialize an inflate stream
    z_stream dstrm;
    z_stream dcopy;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&dcopy, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dstrm);
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compDestLen;
    uLongf outAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 4: Persist compressed bytes to a gzFile, query EOF, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compDestLen);
    int rc_gzeof = gzeof(gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_def_copy_end = deflateEnd(&dcopy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)rc_def_copy_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}