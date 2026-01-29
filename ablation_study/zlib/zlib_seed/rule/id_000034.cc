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
//<ID> 34
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
    unsigned char inbuf[128];
    unsigned char outbuf[256];
    uLong sourceLen = (uLong)sizeof(inbuf);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    gzFile gz = 0;
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(&dstrm, 0, sizeof(dstrm));
    memset(inbuf, 'A', sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(comp, 0, (size_t)cb);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = inbuf;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflateResetKeep(&dstrm);
    inflateCopy(&istrm_copy, &istrm);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Operate & Validate
    deflate(&dstrm, 0);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)sizeof(outbuf);
    inflate(&istrm, 0);
    gzwrite(gz, comp, (unsigned int)dstrm.total_out);
    gzflush(gz, 0);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}