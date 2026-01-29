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
//<ID> 1214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and compute checksum
    const uLong ORIG_LEN = 300UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'M', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(0UL, orig, (uInt)ORIG_LEN);
    Bytef *iobuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(iobuf, 0, (size_t)ORIG_LEN);

    // step 2: Write original data to a gzip file and read it back, then close read handle with gzclose_r
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    gzread(gr, iobuf, (unsigned int)ORIG_LEN);
    uLong roundtrip_adler = adler32(0UL, iobuf, (uInt)ORIG_LEN);
    gzclose_r(gr);

    // step 3: Initialize a deflate stream, adjust parameters, and finish it
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = iobuf;
    dstrm.avail_out = (uInt)ORIG_LEN;
    deflateParams(&dstrm, 9, 0);
    deflateEnd(&dstrm);

    // step 4: Initialize inflateBack context and clean up resources
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateBackInit_(&istrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&istrm);
    free(window);
    free(orig);
    free(iobuf);
    // API sequence test completed successfully
    return 66;
}