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
//<ID> 1298
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
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = inBuf;
    dstrm.avail_in = (uInt)srcLen;
    uLong outBound = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)outBound];
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Core operations
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    gzprintf(gzf, version);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] inBuf;
    delete [] outBuf;
    (void)rc_deflate_init;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}