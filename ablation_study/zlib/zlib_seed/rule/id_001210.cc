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
//<ID> 1210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute checksum
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong checksum = adler32(0UL, orig, (uInt)ORIG_LEN);

    // step 2: Write to a gzip file and close read handle with gzclose_r
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gzw, orig, (unsigned int)ORIG_LEN);
    gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    gzclose_r(gzr);

    // step 3: Initialize deflate stream, adjust parameters, initialize inflate and call inflateBackEnd
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateParams(&dstrm, 9, 1);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateBackEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    // API sequence test completed successfully
    return 66;
}