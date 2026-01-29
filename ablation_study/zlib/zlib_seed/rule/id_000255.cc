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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char srcData[256];
    memset(srcData, 'Z', sizeof(srcData));
    uLong srcLen = (uLong)sizeof(srcData);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&defStrm, srcLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_in = (Bytef *)srcData;
    defStrm.avail_in = (uInt)srcLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;

    // step 3: Core operations
    deflate(&defStrm, 0);
    uLong compSize = defStrm.total_out;
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compSize;
    unsigned char outData[256];
    memset(outData, 0, sizeof(outData));
    infStrm.next_out = (Bytef *)outData;
    infStrm.avail_out = (uInt)sizeof(outData);
    inflate(&infStrm, 0);
    inflateUndermine(&infStrm, 0);
    inflateBackEnd(&infStrm);

    // step 4: Validate & Cleanup
    gzFile f1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(f1, outData, (unsigned int)infStrm.total_out);
    gzclose(f1);
    gzFile f2 = gzdopen(1, "wb");
    gzwrite(f2, compBuf, (unsigned int)compSize);
    gzclose(f2);
    deflateEnd(&defStrm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}