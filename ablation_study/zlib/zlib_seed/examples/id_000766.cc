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
//<ID> 766
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation (Initialize)
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzseek, gzclose and inflateValidate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;

    // step 2: Compress the source into compBuf (Configure -> Operate)
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);
    uLong compProduced = (uLong)compLen;

    // step 3: Write compressed data to a gz file, seek and close (Operate)
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)compProduced);
    off_t rc_gz_seek = gzseek(gf, (off_t)0, 0);
    int rc_gz_close = gzclose(gf);

    // step 4: Prepare a z_stream for validation, validate and cleanup (Validate -> Cleanup)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate_validate = inflateValidate(&istrm, 0);

    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)compProduced;
    (void)rc_gz_write;
    (void)rc_gz_seek;
    (void)rc_gz_close;
    (void)rc_inflate_validate;
    // API sequence test completed successfully
    return 66;
}