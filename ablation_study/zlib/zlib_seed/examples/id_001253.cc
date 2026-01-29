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
//<ID> 1253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Example payload for zlib sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    uLongf compLen_f = (uLongf)bound;
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);

    // step 2: Setup (stateless compress and write to gz)
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)compBuf, (unsigned int)(compLen_f ? compLen_f : 0));
    int rc_gzflush = gzflush(gzf, 0);

    // step 3: Core operations (initialize streams and reset inflate state keeping context)
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)(compLen_f ? compLen_f : 0);
    Bytef *outBuf = new Bytef[(size_t)(bound + 16)];
    memset(outBuf, 0, (size_t)(bound + 16));
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)(bound + 16);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)(compLen_f ? compLen_f : 0);
    int rc_inflate_reset_keep = inflateResetKeep(&istrm);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_inflate_reset_keep;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}