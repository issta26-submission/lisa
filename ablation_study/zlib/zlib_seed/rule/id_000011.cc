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
//<ID> 11
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
    unsigned char *comp_buf = 0;
    gzFile gz = 0;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    memset(inbuf, 'Z', sizeof(inbuf));
    defstrm.next_in = inbuf;
    defstrm.avail_in = (uInt)sizeof(inbuf);
    defstrm.next_out = 0;
    defstrm.avail_out = 0;
    inflateInit_(&instrm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 2: Configure
    deflateResetKeep(&defstrm);
    uLong bound = compressBound((uLong)defstrm.avail_in);
    comp_buf = (unsigned char*)malloc((size_t)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    inflateUndermine(&instrm, 1);

    // step 3: Operate & Validate
    deflate(&defstrm, 0);
    gzputc(gz, (int)comp_buf[0]);
    gzputc(gz, 'K');

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&instrm);
    gzclose(gz);
    free(comp_buf);

    // API sequence test completed successfully
    return 66;
}