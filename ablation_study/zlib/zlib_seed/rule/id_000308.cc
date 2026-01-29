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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&inf_strm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    unsigned char input_data[64];
    memset(input_data, 'Z', sizeof(input_data));
    inf_strm.next_in = (Bytef *)input_data;
    inf_strm.avail_in = (uInt)sizeof(input_data);
    inflateReset(&inf_strm);

    // step 3: Operate & Validate
    inflateUndermine(&inf_strm, 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "inflateUndermine and inflateBackInit_ test");

    // step 4: Cleanup
    inflateEnd(&inf_strm);
    gzclose(gz);
    free(window);

    return 66;
    // API sequence test completed successfully
}