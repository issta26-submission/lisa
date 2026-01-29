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
//<ID> 689
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress it into a buffer
    const char src[] = "zlib API sequence test payload for inflate and gz I/O";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize and configure inflate stream and output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 1;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;

    // step 3: Operate — reset the inflate stream, perform inflate, then write output to a gzFile
    int rc_inf_reset = inflateReset(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    outBuf[(size_t)istrm.total_out] = 0;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputs = gzputs(gf, (const char *)outBuf);
    int rc_gzdirect = gzdirect(gf);
    gzclearerr(gf);

    // step 4: Validate and cleanup
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)rc_gzputs;
    (void)rc_gzdirect;
    (void)rc_inf_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}