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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize buffers
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    unsigned char inbuf[128];
    unsigned char outbuf[512];
    memset(inbuf, 'X', sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    uLong inlen = (uLong)sizeof(inbuf);
    uLong comp_bound = compressBound(inlen);
    Bytef *compBuf = (Bytef *)malloc((size_t)comp_bound);
    memset(compBuf, 0, (size_t)comp_bound);

    // step 2: Initialize deflate, compress some data
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)inbuf;
    dstrm.avail_in = (uInt)inlen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 0);

    // step 3: Reset keep, prepare inflater, copy inflater and inflate, write to gz and rewind
    deflateResetKeep(&dstrm);
    dstrm.next_in = (Bytef *)inbuf;
    dstrm.avail_in = (uInt)inlen;
    dstrm.next_out = compBuf + (uInt)dstrm.total_out;
    dstrm.avail_out = (uInt)(comp_bound - (uLong)dstrm.total_out);
    deflate(&dstrm, 0);

    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = (Bytef *)outbuf;
    istrm.avail_out = (uInt)sizeof(outbuf);

    inflateCopy(&istrm_copy, &istrm);
    inflate(&istrm_copy, 0);
    inflate(&istrm, 0);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)outbuf, (unsigned int)istrm.total_out);
    gzrewind(gz);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, outbuf, (uInt)istrm.total_out);
    (void)checksum;
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    gzclose(gz);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}