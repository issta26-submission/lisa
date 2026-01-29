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
//<ID> 867
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong BUFLEN = 128UL;
    Bytef *inbuf = (Bytef *)malloc((size_t)BUFLEN);
    memset(inbuf, 'Z', (size_t)BUFLEN);
    uLong crc_before = crc32(0UL, inbuf, (uInt)BUFLEN);

    // step 2: Initialize & configure streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    uLong compBound = compressBound(BUFLEN);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    dstrm.next_in = inbuf;
    dstrm.avail_in = (uInt)BUFLEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)compBound;

    // step 3: Core operations & validation
    deflate(&dstrm, 4);
    Bytef *outbuf = (Bytef *)malloc((size_t)BUFLEN);
    memset(outbuf, 0, (size_t)BUFLEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)BUFLEN;
    inflate(&istrm, 4);
    uLong crc_after = crc32(0UL, outbuf, (uInt)BUFLEN);

    gzFile gzf = gzdopen(1, "wb");
    gzwrite(gzf, outbuf, (unsigned int)BUFLEN);
    gzclose(gzf);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(inbuf);
    free(comp);
    free(outbuf);
    (void)ver;
    (void)crc_before;
    (void)crc_after;
    (void)compBound;
    // API sequence test completed successfully
    return 66;
}