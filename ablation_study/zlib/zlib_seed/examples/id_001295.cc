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
//<ID> 1295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflate and gzprintf";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)bound];
    memset(outBuf, 0, (size_t)bound);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = buf;
    strm.avail_in = (uInt)srcLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)bound;

    // step 3: Core operations
    int rc_deflate_call = deflate(&strm, 0);
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzprintf = gzprintf(gzf, "%s", payload);

    // step 4: Cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&strm);
    delete [] buf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_call;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}