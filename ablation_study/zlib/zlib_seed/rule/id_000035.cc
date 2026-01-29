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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    z_stream istrm_copy;
    z_stream dstrm;
    unsigned char input[128];
    unsigned char outBuf[256];
    unsigned char window[32];
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(&dstrm, 0, sizeof(dstrm));
    memset(input, 'A', sizeof(input));
    memset(outBuf, 0, sizeof(outBuf));
    memset(window, 0, sizeof(window));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb+");

    // initialize deflate and inflate (use inflateInit_ so inflateCopy is valid)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflateResetKeep(&dstrm);
    inflateCopy(&istrm_copy, &istrm);

    // step 3: Operate & Validate
    deflate(&dstrm, 0);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sizeof(outBuf);
    inflate(&istrm, 0);
    gzwrite(gz, outBuf, 1);
    gzflush(gz, 0);

    // step 4: Cleanup
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    deflateEnd(&dstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}