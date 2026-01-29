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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib compress/inflate and gz offset check";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure
    int rc_deflate_params = deflateParams(&dstrm, 2, 0);

    // step 3: Operate
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gzf = gzopen64("temp_zlib_api.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidp)source, (unsigned int)sourceLen);
    off64_t off = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate & Cleanup
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    (void)rc_deflate_init; (void)rc_inflate_init; (void)rc_deflate_params;
    (void)rc_comp; (void)rc_gzwrite; (void)off; (void)rc_gzclose;
    (void)rc_inflate; (void)rc_inflate_end; (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}