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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream bstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&bstrm, 0, sizeof(bstrm));
    unsigned char input[128];
    unsigned char outbuf[256];
    memset(input, 'A', sizeof(input));
    memset(outbuf, 0, sizeof(outbuf));
    unsigned char *comp = (unsigned char *)malloc(256);
    memset(comp, 0, 256);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sizeof(input);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)256;
    deflate(&dstrm, 0);

    // step 3: Operate & Validate
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&bstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)comp[0]);

    // step 4: Cleanup
    inflateBackEnd(&bstrm);
    deflateEnd(&dstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}