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
//<ID> 634
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, version, and compression buffer; compress source one-shot
    const char src[] = "zlib API sequence test payload for read/decompress flow";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compBufAlloc = (uLongf)(sourceLen + 256);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufAlloc);
    memset(compBuf, 0, (size_t)compBufAlloc);
    uLongf compDestLen = compBufAlloc;
    int rc_compress2 = compress2(compBuf, &compDestLen, source, sourceLen, 6);

    // step 2: Write compressed bytes to a gz file, then reopen and read them back using gzfread and gzerror
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, compBuf, (unsigned int)compDestLen);
    int rc_gwclose = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    uLongf compBufReadAlloc = compBufAlloc;
    Bytef *compBufRead = (Bytef *)malloc((size_t)compBufReadAlloc);
    memset(compBufRead, 0, (size_t)compBufReadAlloc);
    z_size_t itemsRead = gzfread((voidp)compBufRead, (z_size_t)1, (z_size_t)compBufReadAlloc, gr);
    int gz_errnum = 0;
    const char *gz_errmsg = gzerror(gr, &gz_errnum);

    // step 3: Decompress using uncompress2, then initialize an inflate stream with inflateInit2_ and inflate into a separate buffer
    uLong srcLenForUncomp = (uLong)itemsRead;
    uLongf outAlloc1 = (uLongf)(sourceLen + 128);
    Bytef *outBuf1 = (Bytef *)malloc((size_t)outAlloc1);
    memset(outBuf1, 0, (size_t)outAlloc1);
    int rc_uncompress2 = uncompress2(outBuf1, &outAlloc1, compBufRead, &srcLenForUncomp);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBufRead;
    istrm.avail_in = (uInt)itemsRead;
    uLongf outAlloc2 = (uLongf)(sourceLen + 128);
    Bytef *outBuf2 = (Bytef *)malloc((size_t)outAlloc2);
    memset(outBuf2, 0, (size_t)outAlloc2);
    istrm.next_out = outBuf2;
    istrm.avail_out = (uInt)outAlloc2;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 4: Validate & Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_grclose = gzclose(gr);
    free(compBuf);
    free(compBufRead);
    free(outBuf1);
    free(outBuf2);
    (void)rc_compress2;
    (void)rc_gzwrite;
    (void)rc_gwclose;
    (void)itemsRead;
    (void)gz_errnum;
    (void)gz_errmsg;
    (void)rc_uncompress2;
    (void)rc_inflate_init2;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_grclose;
    // API sequence test completed successfully
    return 66;
}