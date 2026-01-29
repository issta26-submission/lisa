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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> write gz -> deflate -> inflateBackInit_ -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure & Operate (compress and write to gz)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gz_printf = gzprintf(gz, "wrote compressed payload");
    int rc_gz_close = gzclose(gz);

    // step 3: Operate & Validate (initialize deflate, perform a deflate, initialize inflateBack_)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    uLong defBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *defOut = (Bytef *)malloc((size_t)defBound);
    memset(defOut, 0, (size_t)defBound);
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)defBound;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *windowBuf = (unsigned char *)malloc((size_t)(1 << 15));
    memset(windowBuf, 0, (size_t)(1 << 15));
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, windowBuf, version, (int)sizeof(z_stream));
    int rc_inf_back_end = inflateBackEnd(&istrm);

    // step 4: Cleanup
    free(compBuf);
    free(defOut);
    free(windowBuf);
    (void)rc_comp; (void)rc_gz_write; (void)rc_gz_printf; (void)rc_gz_close;
    (void)rc_def_init; (void)rc_deflate; (void)rc_def_end;
    (void)rc_inf_back_init; (void)rc_inf_back_end; (void)bound; (void)defBound;
    // API sequence test completed successfully
    return 66;
}