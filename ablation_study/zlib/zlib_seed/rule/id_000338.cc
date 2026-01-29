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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    Bytef inBuf[256];
    memset(inBuf, 'A', sizeof(inBuf));
    uLong inLen = (uLong)sizeof(inBuf);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Operate (compress2)
    uLong bound = compressBound(inLen);
    uLong allocSize = bound * 2 + 64;
    Bytef * compBuf = (Bytef *)malloc((size_t)allocSize);
    memset(compBuf, 0, (size_t)allocSize);
    uLongf compLen = (uLongf)allocSize;
    compress2(compBuf, &compLen, inBuf, inLen, 6);

    // step 3: Configure & Operate (deflate streaming append)
    dstrm.next_in = inBuf;
    dstrm.avail_in = (uInt)inLen;
    dstrm.next_out = compBuf + compLen;
    dstrm.avail_out = (uInt)(allocSize - compLen);
    deflate(&dstrm, 4);
    uLong producedByDeflate = dstrm.total_out;
    uLong totalCompLen = (uLong)compLen + producedByDeflate;

    // step 4: Inflate, Validate & Cleanup
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)totalCompLen;
    uLong outAlloc = inLen * 2 + 64;
    Bytef * outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    inflateUndermine(&istrm, 1);
    inflate(&istrm, 0);
    unsigned long codes = inflateCodesUsed(&istrm);
    (void)codes;
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclose(gf);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}