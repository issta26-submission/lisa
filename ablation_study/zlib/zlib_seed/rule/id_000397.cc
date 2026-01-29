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
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize data and deflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * outBuf = (Bytef *)malloc((size_t)bound);
    memset(outBuf, 0, (size_t)bound);
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)bound;
    strm.adler = checksum;

    // step 2: Operate - first deflate pass
    deflate(&strm, 0);
    uLong first_out = strm.total_out;

    // step 3: Configure reuse via reset and operate again, then finish deflate
    deflateResetKeep(&strm);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)bound;
    strm.adler = checksum;
    deflate(&strm, 0);
    uLong final_out = strm.total_out;
    deflateEnd(&strm);

    // step 4: Write compressed data to gz file, close and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)final_out);
    gzclose_r(gz);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}