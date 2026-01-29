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
//<ID> 28
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
    z_stream bstrm;
    unsigned char input[128];
    unsigned char decomp[128];
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&bstrm, 0, sizeof(bstrm));
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

    // step 2: Configure
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    unsigned char window[32768];
    inflateBackInit_(&bstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate
    deflate(&dstrm, 0);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)sizeof(decomp);
    inflate(&istrm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzputc(gz, (int)decomp[0]);
    gzrewind(gz);
    int c = gzgetc_(gz);
    (void)c;

    // step 4: Cleanup
    inflateBackEnd(&bstrm);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}