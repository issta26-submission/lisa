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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf;
    z_stream def;
    unsigned char srcBuf[256];
    memset(&inf, 0, sizeof(inf));
    memset(&def, 0, sizeof(def));
    memset(srcBuf, 'Z', sizeof(srcBuf));
    uLong bound = compressBound((uLong)sizeof(srcBuf));
    unsigned char * compBuf = new unsigned char[bound];
    uLongf compLen = bound;
    gzFile gz = 0;
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&def, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflateResetKeep(&def);
    inflateUndermine(&inf, 1);

    // step 3: Operate & Validate
    compress(compBuf, &compLen, srcBuf, (uLong)sizeof(srcBuf));
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)compBuf[0]);

    // step 4: Cleanup
    deflateEnd(&def);
    inflateEnd(&inf);
    gzclose(gz);
    delete [] compBuf;

    // API sequence test completed successfully
    return 66;
}