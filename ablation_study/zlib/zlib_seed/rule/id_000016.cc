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
//<ID> 16
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
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char inbuf[64];
    unsigned char outbuf[128];
    memset(inbuf, 'Z', sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    defstrm.next_in = inbuf;
    defstrm.avail_in = (uInt)sizeof(inbuf);
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)sizeof(outbuf);
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflateResetKeep(&defstrm);
    uLong bound = compressBound((uLong)defstrm.avail_in);
    unsigned char * compbuf = (unsigned char *)malloc((size_t)bound);
    memset(compbuf, 0, (size_t)bound);
    defstrm.next_out = compbuf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Operate & Validate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzputc(gz, inbuf[0]);
    gzputc(gz, inbuf[1]);
    gzputc(gz, inbuf[2]);
    inflateUndermine(&infstrm, 1);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    gzclose(gz);
    free(compbuf);

    // API sequence test completed successfully
    return 66;
}