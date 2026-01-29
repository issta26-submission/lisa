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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    unsigned char input[128];
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *buf = (unsigned char *)malloc((size_t)cb);
    gzFile gz = 0;
    memset(&istrm, 0, sizeof(istrm));
    memset(input, 'Z', sizeof(input));
    memset(buf, 0xAB, (size_t)cb);

    // step 2: Configure
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzwrite(gz, buf, (unsigned int)cb);
    gzrewind(gz);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)cb;

    // step 3: Operate & Validate
    int c = gzgetc_(gz);
    istrm.data_type = c;

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    gzclose(gz);
    free(buf);

    // API sequence test completed successfully
    return 66;
}