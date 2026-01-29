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
//<ID> 1531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char fname[] = "test_zlib_api_sequence_input.gz";
    Bytef *inbuf = new Bytef[4096];
    memset(inbuf, 0, 4096);
    Bytef *outbuf = new Bytef[8192];
    memset(outbuf, 0, 8192);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 2: Configure (open gz file and query compile flags)
    gzFile gzf = gzopen(fname, "rb");
    uLong flags = zlibCompileFlags();

    // step 3: Operate (read compressed data and perform a single inflate call)
    int bytes_read = gzread(gzf, inbuf, (unsigned int)4096);
    strm.next_in = inbuf;
    strm.avail_in = (uInt)bytes_read;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)8192;
    int rc_inflate = inflate(&strm, 0);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    int rc_close = gzclose(gzf);
    delete [] inbuf;
    delete [] outbuf;
    (void)rc_inflate_init;
    (void)flags;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_close;
    (void)version;
    // API sequence test completed successfully
    return 66;
}