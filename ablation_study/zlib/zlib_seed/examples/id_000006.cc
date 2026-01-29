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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    unsigned char src[] = "Hello zlib test data";
    unsigned char compr[256];
    uLong srcLen = (uLong)(sizeof(src) - 1);
    uLong comprLen = (uLong)sizeof(compr);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 2: Setup (Initialize streams)
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    defstrm.next_out = compr;
    defstrm.avail_out = (uInt)comprLen;
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations (compress, check sync point, backend inflate, write gz)
    deflate(&defstrm, 0);
    infstrm.next_in = compr;
    infstrm.avail_in = (uInt)(defstrm.total_out);
    inflateSyncPoint(&infstrm);
    inflateBackEnd(&infstrm);
    gzprintf(gz, "API sequence test completed successfully");

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}