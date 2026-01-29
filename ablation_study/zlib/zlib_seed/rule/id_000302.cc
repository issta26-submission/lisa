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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_strm;
    z_stream back_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&back_strm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    unsigned char input_buf[64];
    memset(input_buf, 'Z', sizeof(input_buf));
    inf_strm.next_in = (Bytef *)input_buf;
    inf_strm.avail_in = (uInt)sizeof(input_buf);
    unsigned char out_buf[128];
    memset(out_buf, 0, sizeof(out_buf));
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)sizeof(out_buf);

    // step 3: Operate & Validate
    inflateReset(&inf_strm);
    inflateUndermine(&inf_strm, 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "zlib test output");
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&inf_strm);
    inflateBackEnd(&back_strm);
    free(window);
    return 66; // API sequence test completed successfully
}