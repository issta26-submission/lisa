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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib payload for API sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);

    // step 2: Setup (compress the source into a compact buffer)
    uLongf compLen_f = (uLongf)compressBound(srcLen);
    uLong compBound = (uLong)compLen_f;
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;

    // step 3: Core operations (initialize streams, reset/validate inflate, prepare deflate, write gz file and flush)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.total_in = compLen;
    int rc_inflate_resetkeep = inflateResetKeep(&istrm);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    uLong dbound = deflateBound(&dstrm, srcLen);
    size_t dOutSize = (size_t)(dbound + 1UL);
    Bytef *dOut = new Bytef[dOutSize];
    memset(dOut, 0, dOutSize);
    dstrm.next_out = dOut;
    dstrm.avail_out = (uInt)dOutSize;

    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] dOut;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_resetkeep;
    (void)rc_deflate_init;
    (void)dbound;
    (void)dOutSize;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}