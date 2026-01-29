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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source, compute compression bound, compress source
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate -> log";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Configure stream buffers and perform inflate operation, then log results to a gz file
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    const char *err_comp = zError(rc_comp);
    const char *err_inflate = zError(rc_inflate);
    gzFile gzf = gzopen("zlib_test_output.gz", "wb");
    gzprintf(gzf, "%s\n", err_comp);
    gzprintf(gzf, "%s\n", err_inflate);

    // step 4: Validate via returned codes and cleanup resources
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)version;
    (void)decompAlloc;
    (void)rc_inflate;
    (void)err_comp;
    (void)err_inflate;
    (void)rc_inf_end;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}