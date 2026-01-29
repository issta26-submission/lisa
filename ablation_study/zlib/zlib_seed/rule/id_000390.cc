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
//<ID> 390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Initialize deflate and allocate output buffer
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;
    strm.adler = checksum;
    deflateResetKeep(&strm);

    // step 3: Operate — perform a deflate and write compressed output to a gz file
    deflate(&strm, 0);
    uLong outBytes = strm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)outBytes);
    gzclose_r(gz);

    // step 4: Cleanup and finalize
    deflateEnd(&strm);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}