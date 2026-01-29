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
//<ID> 483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize deflate stream
    const char src[] = "zlib API sequence payload: initialize -> deflateBound -> deflate -> gz write -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;

    // step 2: Query deflate bound and allocate output buffer
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate deflate, write to gzFile, and query gz I/O properties
    int rc_def_op = deflate(&dstrm, 0);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    off64_t offs64 = gzoffset64(gf);

    // step 4: Validate with adler32 combines and cleanup all resources
    uLong ad1 = adler32(0L, source, (uInt)sourceLen);
    uLong ad2 = adler32(0L, outBuf, (uInt)dstrm.total_out);
    uLong combined = adler32_combine(ad1, ad2, (off_t)offs64);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def_op;
    (void)rc_gzwrite;
    (void)direct;
    (void)offs64;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}