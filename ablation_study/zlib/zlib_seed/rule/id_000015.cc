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
//<ID> 15
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
    unsigned char src[16];
    unsigned char buffer[256];
    gzFile gz = 0;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    memset(src, 'A', sizeof(src));
    memset(buffer, 0, sizeof(buffer));
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound((uLong)sizeof(src));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)sizeof(src);
    defstrm.next_out = buffer;
    defstrm.avail_out = (uInt)sizeof(buffer);
    deflateResetKeep(&defstrm);
    inflateUndermine(&infstrm, 1);

    // step 3: Operate & Validate
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzputc(gz, (int)(bound & 0xFF));
    gzputc(gz, (int)src[0]);

    // step 4: Cleanup
    inflateEnd(&infstrm);
    deflateEnd(&defstrm);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}