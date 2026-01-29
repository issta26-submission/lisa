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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    unsigned char input[128];
    unsigned char outbuf[128];
    gzFile gz = 0;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    memset(input, 'A', sizeof(input));
    memset(outbuf, 0, sizeof(outbuf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char * comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    defstrm.next_in = input;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = comp;
    defstrm.avail_out = (uInt)cb;
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflateResetKeep(&defstrm);
    inflateResetKeep(&infstrm);

    // step 3: Operate & Validate
    deflate(&defstrm, 0);
    infstrm.next_in = comp;
    infstrm.avail_in = (uInt)defstrm.total_out;
    infstrm.next_out = outbuf;
    infstrm.avail_out = (uInt)sizeof(outbuf);
    inflate(&infstrm, 0);
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzgetc_(gz);
    inflateBackEnd(&infstrm);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}