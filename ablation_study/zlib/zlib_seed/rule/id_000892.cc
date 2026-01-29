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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(comp, &compLen, orig, ORIG_LEN, 6);

    // step 2: Initialize and perform deflate on the original buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    Bytef *defout = (Bytef *)malloc((size_t)bound);
    dstrm.next_out = defout;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);

    // step 3: Inflate the compressed data produced by compress2 and then call inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateBackEnd(&istrm);

    // step 4: Use gzFile APIs and cleanup
    gzFile gf = gzopen("/dev/null", "rb");
    int ch = gzgetc_(gf);
    gzclose(gf);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(defout);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)compLen;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}