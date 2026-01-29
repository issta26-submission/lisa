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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Configure
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&inf_strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sizeof(outBuf);

    // step 3: Operate & Validate
    inflateReset(&inf_strm);
    inflateUndermine(&inf_strm, 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&inf_strm);
    free(compBuf);
    free(window);
    return 66;
    // API sequence test completed successfully
}