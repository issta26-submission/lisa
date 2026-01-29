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
//<ID> 648
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress
    const char src[] = "zlib API sequence test payload to exercise compress and streams";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream with deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    // step 3: Operate deflate on the already-compressed buffer (flowing data through streams)
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    uLongf outAlloc = compAlloc + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outAlloc;
    int rc_deflate = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Initialize inflate stream, set a dictionary, persist compressed bytes via gzdopen and close with gzclose_r, cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_setdict = inflateSetDictionary(&istrm, source, (uInt)sourceLen);
    int rc_inf_end = inflateEnd(&istrm);
    gzFile gf = gzdopen(1, "wb"); /* wrap stdout descriptor for persistence */
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzclose_r = gzclose_r(gf);

    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_def_init2;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}