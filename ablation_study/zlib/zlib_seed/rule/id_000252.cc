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
//<ID> 252
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
    memset(rawData, 'X', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, rawLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound ? (size_t)bound : 1);
    memset(compBuf, 0, (size_t)bound ? (size_t)bound : 1);

    // step 2: Setup / Configure
    dstrm.next_in = (Bytef *)rawData;
    dstrm.avail_in = (uInt)rawLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    gzFile gz_stdout = gzdopen(1, "wb");
    gzFile gz_file = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz_stdout, compBuf, (unsigned int)dstrm.total_out);
    gzwrite(gz_file, compBuf, (unsigned int)dstrm.total_out);

    // step 3: Operate / Validate
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    gzclose(gz_stdout);
    gzclose(gz_file);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}