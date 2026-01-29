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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize data and compression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;

    // step 2: Configure (reset while keeping any allocated state and set checksum)
    deflateResetKeep(&strm);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    strm.adler = checksum;
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate (perform compression operations and write compressed data to gz)
    deflate(&strm, 0);
    deflate(&strm, 4);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)strm.total_out);
    gzclose_r(gz);

    // step 4: Validate-like cleanup and free resources
    deflateEnd(&strm);
    free(compBuf);
    return 66; // API sequence test completed successfully
}