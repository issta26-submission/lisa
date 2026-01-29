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
//<ID> 396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input and checksum
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Initialize deflate stream and allocate output buffer
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.adler = checksum;
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate (reset-keep + deflate) and write compressed data to gz files (write then read)
    deflateResetKeep(&strm);
    deflate(&strm, 0);
    uInt usedOut = (uInt)((uLong)bound - (uLong)strm.avail_out);
    gzFile gzW = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzW, compBuf, usedOut);
    gzclose(gzW);
    gzFile gzR = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclose_r(gzR);

    // step 4: Cleanup and teardown
    deflateEnd(&strm);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}