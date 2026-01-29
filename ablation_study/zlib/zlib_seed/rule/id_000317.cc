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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[64];
    memset(input, 'X', sizeof(input));
    unsigned char outbuf[128];
    memset(outbuf, 0, sizeof(outbuf));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)sizeof(input);
    strm.next_out = (Bytef *)outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflate(&strm, 4);
    deflateResetKeep(&strm);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)sizeof(input);
    strm.next_out = (Bytef *)outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Operate & Validate
    deflate(&strm, 4);
    uInt compLen = (uInt)strm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)outbuf[0]);
    gzputc(gz, (int)outbuf[compLen > 1 ? 1 : 0]);
    gzputc(gz, '\n');
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&strm);
    return 66;
    // API sequence test completed successfully
}