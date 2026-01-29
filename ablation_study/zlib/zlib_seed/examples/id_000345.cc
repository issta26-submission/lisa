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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare version and payload
    const char *version = zlibVersion();
    const char src[] = "zlib API sequence payload: write with gzwrite, read with gzread, then inflate lifecycle";
    const unsigned int srcLen = (unsigned int)(sizeof(src) - 1);

    // step 2: Write payload to a gzip file
    gzFile wfile = gzopen("tmp_zlib_api.gz", "wb");
    int rc_write = gzwrite(wfile, (voidpc)src, srcLen);
    int rc_close_w = gzclose(wfile);

    // step 3: Read payload back from the gzip file
    gzFile rfile = gzopen("tmp_zlib_api.gz", "rb");
    uLongf readBufSize = 512;
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufSize);
    memset(readBuf, 0, (size_t)readBufSize);
    int rc_read = gzread(rfile, (voidp)readBuf, (unsigned int)readBufSize);
    int rc_close_r = gzclose(rfile);

    // step 4: Initialize inflate stream, attempt inflate using read buffer, then cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = readBuf;
    strm.avail_in = (uInt)rc_read;
    uLongf outBufSize = 1024;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outBuf, 0, (size_t)outBufSize);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)outBufSize;
    int rc_inflate = inflate(&strm, 0);
    int rc_inf_end = inflateEnd(&strm);

    free(readBuf);
    free(outBuf);

    (void)version;
    (void)rc_write;
    (void)rc_close_w;
    (void)rc_read;
    (void)rc_close_r;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}