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
//<ID> 1162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare version string and zero-initialize inflate stream
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;

    // step 2: Initialize inflate with the library version info
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Configure input for the stream and perform resets to exercise state-management APIs
    istrm.next_in = (Bytef *)ver;
    istrm.avail_in = (uInt)strlen(ver);
    inflateReset(&istrm);
    inflateResetKeep(&istrm);

    // step 4: Write version to a gzip file and clean up inflate state
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzputs(gf, ver);
    gzclose(gf);
    inflateEnd(&istrm);

    // API sequence test completed successfully
    return 66;
}