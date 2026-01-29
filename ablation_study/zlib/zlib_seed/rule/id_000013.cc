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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream instrm;
    unsigned char inbuf[128];
    unsigned char compbuf[1024];
    gzFile gz = 0;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    memset(inbuf, 'A', sizeof(inbuf));
    memset(compbuf, 0, sizeof(compbuf));
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = compressBound((uLong)sizeof(inbuf));
    defstrm.next_in = inbuf;
    defstrm.avail_in = (uInt)sizeof(inbuf);
    defstrm.next_out = compbuf;
    defstrm.avail_out = (uInt)bound;

    // step 2: Configure
    deflateResetKeep(&defstrm);
    inflateInit_(&instrm, zlibVersion(), (int)sizeof(z_stream));
    inflateUndermine(&instrm, 1);

    // step 3: Operate & Validate
    deflate(&defstrm, 0);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)inbuf[0]);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&instrm);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}