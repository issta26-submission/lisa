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
//<ID> 265
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
    gz_header header;
    memset(&header, 0, sizeof(header));
    unsigned char rawData[128];
    memset(rawData, 'A', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);

    // step 2: Configure
    Bytef compBuf[512];
    memset(compBuf, 0, sizeof(compBuf));
    uLongf compLen = (uLongf)sizeof(compBuf);
    compress2(compBuf, &compLen, rawData, rawLen, 6);
    Bytef outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    uLongf outLen = (uLongf)sizeof(outBuf);

    // step 3: Operate
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)outLen;
    inflateSync(&inf_strm);
    inflateGetHeader(&inf_strm, &header);
    inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)inf_strm.total_out);
    gzclose(gz);
    inflateEnd(&inf_strm);

    return 66;
    // API sequence test completed successfully
}