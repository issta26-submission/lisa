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
//<ID> 318
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&strm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate & Validate
    deflate(&strm, 4);
    uInt firstCompLen = (uInt)strm.total_out;
    deflateResetKeep(&strm);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;
    strm.total_out = 0;
    deflate(&strm, 4);
    uInt secondCompLen = (uInt)strm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)compBuf[0]);
    gzputc(gz, (int)compBuf[1]);
    gzputc(gz, (int)compBuf[2]);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&strm);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}