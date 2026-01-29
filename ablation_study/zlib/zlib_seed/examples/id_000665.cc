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
//<ID> 665
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence test payload for inflate and gz operations";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compDestLen = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &compDestLen, source, sourceLen);

    // step 2: Initialize inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compDestLen;
    uLong outAlloc = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;

    // step 3: Validate stream, perform inflate, and persist compressed bytes to gzFile
    int rc_validate = inflateValidate(&istrm, 15);
    int rc_inflate = inflate(&istrm, 0);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compDestLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup resources
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_inf_init;
    (void)rc_validate;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}