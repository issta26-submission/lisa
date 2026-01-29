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
//<ID> 14
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
    unsigned char input[64];
    unsigned char comp[256];
    unsigned char decomp[64];
    gzFile gz = 0;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    memset(input, 'Z', sizeof(input));
    memset(comp, 0, sizeof(comp));
    memset(decomp, 0, sizeof(decomp));
    defstrm.next_in = input;
    defstrm.avail_in = (uInt)sizeof(input);
    defstrm.next_out = comp;
    defstrm.avail_out = (uInt)sizeof(comp);
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflateResetKeep(&defstrm);
    uLong bound = compressBound((uLong)defstrm.avail_in);
    defstrm.avail_out = (uInt)((bound <= (uLong)sizeof(comp)) ? bound : (uLong)sizeof(comp));
    infstrm.next_out = decomp;
    infstrm.avail_out = (uInt)sizeof(decomp);

    // step 3: Operate & Validate
    deflate(&defstrm, 4);
    infstrm.next_in = comp;
    infstrm.avail_in = (uInt)defstrm.total_out;
    inflateUndermine(&infstrm, 1);
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzputc(gz, (int)comp[0]);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}