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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress it one-shot
    const char src[] = "zlib API sequence payload: compress2 -> inflateInit_ -> inflate -> gzwrite -> gzread -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and perform inflation of the compressed buffer
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
    uLong produced = strm.total_out;
    int rc_inflate_end = inflateEnd(&strm);

    // step 3: Write the inflated data to a gz stream opened on /dev/null (avoids blocking) and then close
    gzFile gz_out = gzopen("/dev/null", "wb");
    int rc_gzwrite = gzwrite(gz_out, outBuf, (unsigned int)produced);
    int rc_gzclose_out = gzclose(gz_out);

    // step 4: Attempt to read from /dev/null via gzopen and report error string via zError, then cleanup
    gzFile gz_in = gzopen("/dev/null", "rb");
    unsigned char readBuf[32];
    memset(readBuf, 0, sizeof(readBuf));
    int rc_gzread = gzread(gz_in, readBuf, (unsigned int)sizeof(readBuf));
    int rc_gzclose_in = gzclose(gz_in);
    const char *errstr = zError(rc_gzread);

    free(compBuf);
    free(outBuf);
    (void)version;
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)produced;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_out;
    (void)rc_gzread;
    (void)rc_gzclose_in;
    (void)errstr;
    // API sequence test completed successfully
    return 66;
}