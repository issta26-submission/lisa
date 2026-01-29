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
//<ID> 552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream inf1;
    memset(&inf1, 0, sizeof(inf1));
    z_stream inf2;
    memset(&inf2, 0, sizeof(inf2));
    unsigned char sample_in[64];
    memset(sample_in, 0x5A, sizeof(sample_in));
    uInt in_len = (uInt)sizeof(sample_in);
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Initialize & configure inflaters
    inflateInit_(&inf1, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&inf2, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & validate
    inf1.next_in = (Bytef *)sample_in;
    inf1.avail_in = in_len;
    inflateSync(&inf1);
    inf2.next_in = (Bytef *)sample_in;
    inf2.avail_in = in_len;

    // step 4: Cleanup
    inflateEnd(&inf1);
    inflateBackEnd(&inf2);
    free(window);

    // API sequence test completed successfully
    return 66;
}