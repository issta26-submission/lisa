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
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // step 1: Prepare payload and write compressed-style data to a non-blocking sink (/dev/null)
    const char src[] = "zlib API sequence payload: gzdopen -> gzwrite -> gzread -> inflateInit_ -> inflateEnd -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    gzFile gz_out = gzopen("/dev/null", "wb");
    int rc_gzwrite = gzwrite(gz_out, (voidpc)source, (unsigned int)sourceLen);

    // step 2: Initialize an inflate stream and configure it
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;

    // step 3: Open a gz descriptor for reading from a non-blocking source (/dev/null) and attempt a read, then retrieve an error string
    gzFile gz_in = gzopen("/dev/null", "rb");
    char readbuf[128];
    memset(readbuf, 0, sizeof(readbuf));
    int rc_gzread = gzread(gz_in, (voidp)readbuf, (unsigned int)(sizeof(readbuf)));
    const char *errstr = zError(rc_gzread);

    // step 4: Cleanup inflate stream and gz descriptors
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gzclose_out = gzclose(gz_out);
    int rc_gzclose_in = gzclose(gz_in);

    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_gzread;
    (void)errstr;
    (void)rc_inflate_end;
    (void)rc_gzclose_out;
    (void)rc_gzclose_in;
    (void)version;
    // API sequence test completed successfully
    return 66;
}