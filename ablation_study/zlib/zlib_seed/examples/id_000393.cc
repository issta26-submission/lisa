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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress it one-shot
    const char src[] = "zlib API sequence payload: compress2 -> inflateInit_ -> inflate -> gzdopen -> gzwrite -> gzread -> inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize an inflate stream and attempt streaming inflate of the compressed data
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);

    // step 3: Open a gz stream on stdout descriptor and write the compressed data, then attempt a read
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    char readBuf[64];
    memset(readBuf, 0, sizeof(readBuf));
    int rc_gzread = gzread(gzf, readBuf, (unsigned int)sizeof(readBuf));
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup inflate stream and buffers, and map an error code to text
    int rc_inflate_end = inflateEnd(&strm);
    const char *errstr = zError(rc_gzread);
    free(compBuf);
    free(outBuf);
    (void)version;
    (void)rc_comp2;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    (void)errstr;
    // API sequence test completed successfully
    return 66;
}