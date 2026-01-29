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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32_z(0UL, (const Bytef *)input, (z_size_t)inputLen);
    uLong outBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)outBound);
    memset(compBuf, 0, (size_t)outBound);

    // step 2: Initialize and configure deflate stream
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)outBound;
    strm.adler = checksum;

    // step 3: Operate (deflate) and write compressed data to gz
    deflate(&strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)strm.total_out);
    gzputc(gz, 'Z');

    // step 4: Cleanup and validate-like teardown
    gzclose_w(gz);
    deflateEnd(&strm);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}