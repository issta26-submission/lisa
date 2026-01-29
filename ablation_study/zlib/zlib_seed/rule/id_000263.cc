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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    gz_header head;
    memset(&head, 0, sizeof(head));
    unsigned char compBuf[256];
    memset(compBuf, 0, sizeof(compBuf));
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    strm.next_in = (Bytef *)compBuf;
    strm.avail_in = (uInt)sizeof(compBuf);
    strm.next_out = (Bytef *)outBuf;
    strm.avail_out = (uInt)sizeof(outBuf);
    inflateInit2_(&strm, 15 + 16, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    inflateGetHeader(&strm, &head);

    // step 3: Operate
    inflateSync(&strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "METADATA:inflate_header_test\n");
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&strm);

    // API sequence test completed successfully
    return 66;
}