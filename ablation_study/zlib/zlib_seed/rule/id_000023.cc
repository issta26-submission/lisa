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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    unsigned char input[128];
    unsigned char decomp[256];
    gzFile gz = 0;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    deflateResetKeep(&dstrm);
    inflateResetKeep(&istrm);

    // step 3: Operate & Validate
    deflate(&dstrm, 0);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)sizeof(decomp);
    inflate(&istrm, 0);
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzwrite(gz, decomp, 2);
    gzrewind(gz);
    int c = gzgetc_(gz);
    (void)c;
    inflateBackEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}