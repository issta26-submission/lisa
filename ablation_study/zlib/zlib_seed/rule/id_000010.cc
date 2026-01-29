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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    unsigned char inbuf[64];
    unsigned char outbuf[128];
    gzFile gz = 0;
    memset(&strm, 0, sizeof(strm));
    memset(inbuf, 'Z', sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    strm.next_in = inbuf;
    strm.avail_in = (uInt)sizeof(inbuf);
    uLong bound = compressBound((uLong)sizeof(inbuf));
    strm.next_out = outbuf;
    strm.avail_out = (uInt)bound;
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflateResetKeep(&strm);
    inflateUndermine(&strm, 1);

    // step 3: Operate
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'A');

    // step 4: Cleanup
    inflateEnd(&strm);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}