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
//<ID> 894
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *defbuf = (Bytef *)malloc((size_t)bound);
    memset(defbuf, 0, (size_t)bound);

    // step 2: Initialize deflate stream and compress data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = defbuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong deflen = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Write compressed data to a gzFile and read a byte via gzgetc_
    gzFile gw = gzopen("/dev/null", "wb");
    gzwrite(gw, defbuf, (unsigned int)deflen);
    gzclose(gw);
    gzFile gr = gzopen("/dev/null", "rb");
    int ch = gzgetc_(gr);
    gzclose(gr);

    // step 4: Initialize an inflate stream, call inflateBackEnd, cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    free(orig);
    free(defbuf);
    (void)ver;
    (void)bound;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}