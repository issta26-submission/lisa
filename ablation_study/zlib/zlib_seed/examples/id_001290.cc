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
//<ID> 1290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzprintf and deflate";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = buf;
    defstrm.avail_in = (uInt)srcLen;
    uLong bound = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)bound];
    memset(outBuf, 0, (size_t)bound);
    defstrm.next_out = outBuf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Core operations
    int rc_deflate = deflate(&defstrm, 0);
    uLong ad = adler32(0UL, buf, (uInt)srcLen);
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzprintf1 = gzprintf(gzf, "payload: %s\n", (const char *)buf);
    int rc_gzprintf2 = gzprintf(gzf, "adler32: %lu\n", (unsigned long)ad);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] buf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_gzprintf1;
    (void)rc_gzprintf2;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}