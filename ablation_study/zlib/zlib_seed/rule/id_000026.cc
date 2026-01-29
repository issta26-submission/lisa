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
//<ID> 26
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
    unsigned char source[128];
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(source, 'A', sizeof(source));
    uLong sourceLen = (uLong)sizeof(source);
    uLong bound = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    gzFile gz = 0;

    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate & Validate
    deflate(&dstrm, 0);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp, (unsigned int)dstrm.total_out);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    int c = gzgetc_(gz);
    (void)c;
    inflateBackEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}