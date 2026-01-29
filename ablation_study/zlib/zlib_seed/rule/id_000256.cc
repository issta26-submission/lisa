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
//<ID> 256
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
    unsigned char rawData[256];
    memset(rawData, 'Z', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);
    uLong compBound = compressBound(rawLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef * outBuf = (Bytef *)malloc((size_t)rawLen);
    memset(outBuf, 0, (size_t)rawLen);

    // step 2: Setup / Configure
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)rawData;
    dstrm.avail_in = (uInt)rawLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;

    // step 3: Operate / Validate
    deflate(&dstrm, 0);
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz1, compBuf, (unsigned int)dstrm.total_out);
    gzclose(gz1);
    gzFile gz2 = gzdopen(1, "wb");
    gzwrite(gz2, compBuf, (unsigned int)dstrm.total_out);
    gzclose(gz2);

    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)rawLen;
    inflate(&istrm, 0);
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}