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
//<ID> 261
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
    gz_header header;
    memset(&header, 0, sizeof(header));
    unsigned char rawData[128];
    memset(rawData, 'X', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);
    Bytef compBuf[256];
    uLongf compLen = (uLongf)sizeof(compBuf);
    memset(compBuf, 0, (size_t)compLen);
    compress2(compBuf, &compLen, rawData, rawLen, 6);

    // step 2: Configure & Operate
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    unsigned char outBuf[128];
    memset(outBuf, 0, sizeof(outBuf));
    strm.next_out = outBuf;
    strm.avail_out = (uInt)sizeof(outBuf);
    inflateGetHeader(&strm, &header);
    inflateSync(&strm);
    inflate(&strm, 0);

    // step 3: Validate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)strm.total_out);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&strm);

    // API sequence test completed successfully
    return 66;
}