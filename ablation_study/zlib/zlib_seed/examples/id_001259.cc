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
//<ID> 1259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Sample payload for zlib API sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;

    // step 2: Setup (compress and prepare streams)
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;
    z_stream defstrm;
    z_stream instrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    Bytef *defOut = new Bytef[(size_t)(compLen + 16UL)];
    memset(defOut, 0, (size_t)(compLen + 16UL));
    defstrm.next_in = compBuf;
    defstrm.avail_in = (uInt)compLen;
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)(compLen + 16UL);

    // step 3: Core operations (inflate reset-keep and gz I/O + flush)
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_inflate_reset_keep = inflateResetKeep(&instrm);
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_inflate_end = inflateEnd(&instrm);
    delete [] src;
    delete [] compBuf;
    delete [] defOut;
    (void)version;
    (void)rc_compress;
    (void)bound;
    (void)compLen_f;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_inflate_reset_keep;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}