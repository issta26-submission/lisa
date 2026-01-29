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
//<ID> 1293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzprintf and deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *inBuf = new Bytef[(size_t)srcLen];
    memset(inBuf, 0, (size_t)srcLen);
    memcpy(inBuf, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong outBound = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)outBound];
    memset(outBuf, 0, (size_t)outBound);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = inBuf;
    strm.avail_in = (uInt)srcLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)outBound;

    // step 3: Core operations (write human-readable payload to gzip and keep compression state)
    gzFile gzf = gzopen("zlib_sequence_out.gz", "wb");
    int rc_gzprintf = gzprintf(gzf, "%s", payload);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&strm);
    delete [] inBuf;
    delete [] outBuf;
    (void)version;
    (void)outBound;
    (void)rc_deflate_init;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}