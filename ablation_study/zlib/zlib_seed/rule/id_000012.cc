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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    unsigned char inbuf[64];
    unsigned char outbuf[256];
    gzFile gz;
    int res_deflate_init;
    int res_inflate_init;
    uLong bound;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    memset(inbuf, 'A', sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    defstrm.next_in = inbuf;
    defstrm.avail_in = (uInt)sizeof(inbuf);
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)sizeof(outbuf);
    infstrm.next_in = outbuf;
    infstrm.avail_in = 0;
    infstrm.next_out = outbuf;
    infstrm.avail_out = (uInt)sizeof(outbuf);
    res_deflate_init = deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    res_inflate_init = inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Validate
    deflateResetKeep(&defstrm);
    bound = compressBound((uLong)defstrm.avail_in);
    infstrm.avail_out = (uInt)bound;
    inflateUndermine(&infstrm, 1);

    // step 3: Operate
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)outbuf[0]);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}