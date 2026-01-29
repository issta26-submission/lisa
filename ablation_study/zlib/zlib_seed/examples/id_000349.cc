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
//<ID> 349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload
    const char payload[] = "zlib API sequence payload: gzopen -> gzwrite -> gzread -> inflateEnd usage";
    const Bytef *src = (const Bytef *)payload;
    unsigned int srcLen = (unsigned int)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    const char *fname = "test_zlib_api_sequence.gz";

    // step 2: Write payload to a gzip file
    gzFile wfile = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(wfile, (voidpc)src, srcLen);
    int rc_gzclose_w = gzclose(wfile);

    // step 3: Read payload back from the gzip file
    gzFile rfile = gzopen(fname, "rb");
    unsigned int readBufLen = srcLen + 64;
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufLen);
    memset(readBuf, 0, (size_t)readBufLen);
    int rc_gzread = gzread(rfile, (voidp)readBuf, readBufLen);

    // step 4: Initialize an inflate stream then end it, and perform cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gzclose_r = gzclose(rfile);
    free(readBuf);

    (void)version;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;

    // API sequence test completed successfully
    return 66;
}