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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const char src[] = "zlib API sequence payload: gzopen -> gzwrite -> gzread with inflate init/end";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    const char fname[] = "test_zlib_api_sequence_tmp.gz";
    unsigned int bufSize = (unsigned int)(sourceLen + 64);
    voidp readBuf = malloc((size_t)bufSize);
    memset(readBuf, 0, (size_t)bufSize);

    // step 2: Write the payload to a gzip file and read it back
    gzFile wfile = gzopen(fname, "wb");
    int written = gzwrite(wfile, (voidpc)source, (unsigned int)sourceLen);
    int close_w = gzclose(wfile);
    gzFile rfile = gzopen(fname, "rb");
    int read_bytes = gzread(rfile, readBuf, bufSize);
    int close_r = gzclose(rfile);

    // step 3: Initialize inflate stream using zlib version and then end it
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Validation (capture values) and cleanup
    (void)written;
    (void)close_w;
    (void)read_bytes;
    (void)close_r;
    (void)rc_inf_init;
    (void)rc_inf_end;
    (void)version;
    (void)source;
    (void)sourceLen;
    free(readBuf);

    // API sequence test completed successfully
    return 66;
}