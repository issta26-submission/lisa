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
//<ID> 314
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
    deflateResetKeep(&strm);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;
    deflate(&strm, 4);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'A');

    // step 4: Cleanup
    deflateEnd(&strm);
    gzclose(gz);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}