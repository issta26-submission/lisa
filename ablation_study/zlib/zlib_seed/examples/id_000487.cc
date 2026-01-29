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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and prepare deflate stream
    const char src[] = "zlib API sequence payload: initialize -> deflateBound -> deflate -> gz IO -> checks -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;

    // step 2: Compute output bound, allocate output buffer and run deflate
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);

    // step 3: Write deflate output to a gzFile, query direct mode and offset, combine checksums
    uLong ad1 = adler32(0L, source, (uInt)sourceLen);
    uLong ad2 = adler32(0L, outBuf, (uInt)dstrm.total_out);
    gzFile gf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int wrote = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    off64_t off64 = gzoffset64(gf);
    uLong combined = adler32_combine(ad1, ad2, (off_t)off64);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup streams and memory
    int rc_def_end = deflateEnd(&dstrm);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)wrote;
    (void)direct;
    (void)off64;
    (void)combined;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}