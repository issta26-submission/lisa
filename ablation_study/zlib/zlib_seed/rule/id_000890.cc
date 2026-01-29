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
//<ID> 890
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *defout = (Bytef *)malloc((size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);

    // step 2: Initialize and run deflate to produce compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = defout;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compSize = dstrm.total_out;

    // step 3: Initialize inflate, decompress the deflated data, and call inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = defout;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateBackEnd(&istrm);

    // step 4: Use gzFile internal getc and cleanup all resources
    gzFile gf = gzopen("/dev/null", "rb");
    int ch = gzgetc_(gf);
    gzclose(gf);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    free(defout);
    free(decomp);
    (void)ver;
    (void)bound;
    (void)compSize;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}