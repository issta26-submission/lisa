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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream copy;
    unsigned char inbuf[128];
    unsigned char outbuf[128];
    gzFile gz = 0;
    memset(&src, 0, sizeof(src));
    memset(&copy, 0, sizeof(copy));
    memset(inbuf, 'A', sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    src.next_in = inbuf;
    src.avail_in = (uInt)sizeof(inbuf);
    src.next_out = outbuf;
    src.avail_out = (uInt)sizeof(outbuf);
    inflateInit_(&src, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflateResetKeep(&src);
    inflateCopy(&copy, &src);

    // step 3: Operate & Validate
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzclearerr(gz);
    inflateBack(&src, (in_func)0, (void *)0, (out_func)gzwrite, (void *)gz);
    inflateBackEnd(&src);

    // step 4: Cleanup
    inflateEnd(&src);
    inflateEnd(&copy);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}