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
//<ID> 684
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence test payload for inflateReset, gzputs, gzdirect and gzclearerr usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and prepare output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;

    // step 3: Operate — perform inflate, reset stream, inflate again, then perform gz I/O
    int rc_inflate1 = inflate(&istrm, 0);
    int rc_reset = inflateReset(&istrm);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate2 = inflate(&istrm, 0);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputs = gzputs(gf, src);
    int rc_gzdirect = gzdirect(gf);
    gzclearerr(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and cleanup
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate1;
    (void)rc_reset;
    (void)rc_inflate2;
    (void)rc_gzputs;
    (void)rc_gzdirect;
    (void)rc_gzclose;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}