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
//<ID> 393
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
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    strm.adler = checksum;
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&strm, inputLen);
    Bytef * outBuf = (Bytef *)malloc((size_t)bound);
    memset(outBuf, 0, (size_t)bound);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate & Validate
    deflate(&strm, 0);
    deflateResetKeep(&strm);
    deflate(&strm, 0);
    uLong compressed = strm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)compressed);
    gzclose_r(gz);

    // step 4: Cleanup
    deflateEnd(&strm);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}