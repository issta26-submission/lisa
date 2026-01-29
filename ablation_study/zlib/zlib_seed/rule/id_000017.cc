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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    z_stream dstrm;
    unsigned char inbuf[128];
    unsigned char *compbuf = 0;
    gzFile gz = 0;
    memset(&istrm, 0, sizeof(istrm));
    memset(&dstrm, 0, sizeof(dstrm));
    memset(inbuf, 'A', sizeof(inbuf));
    istrm.next_in = inbuf;
    istrm.avail_in = (uInt)sizeof(inbuf);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound((uLong)istrm.avail_in);
    compbuf = (unsigned char*)new unsigned char[bound];
    dstrm.next_in = istrm.next_in;
    dstrm.avail_in = istrm.avail_in;
    dstrm.next_out = compbuf;
    dstrm.avail_out = (uInt)bound;
    deflateResetKeep(&dstrm);
    inflateUndermine(&istrm, 1);

    // step 3: Operate & Validate
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzputc(gz, (int)inbuf[0]);
    gzputc(gz, (int)inbuf[1]);

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    gzclose(gz);
    delete [] compbuf;

    // API sequence test completed successfully
    return 66;
}