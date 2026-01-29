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
//<ID> 1403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload demonstrating deflate/inflate/init/end/backend usage";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup - initialize deflate stream and allocate output buffer
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)defBound];
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;

    // step 3: Core operations - perform deflate, then initialize inflate and perform inflate
    int rc_deflate = deflate(&dstrm, 0);
    uLong deflated_size = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = defBuf;
    istrm.avail_in = (uInt)deflated_size;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and Cleanup - end inflate, attempt inflateBackEnd cleanup, free allocations
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    delete [] src;
    delete [] defBuf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)deflated_size;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}