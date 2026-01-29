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
//<ID> 257
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
    unsigned char rawData[256];
    memset(rawData, 'Z', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    Bytef presetDict[32];
    memset(presetDict, 'D', sizeof(presetDict));
    deflateSetDictionary(&defStrm, presetDict, (uInt)sizeof(presetDict));
    inflateUndermine(&infStrm, 1);

    // step 3: Core operations
    uLong bound = deflateBound(&defStrm, rawLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_in = (Bytef *)rawData;
    defStrm.avail_in = (uInt)rawLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, 0);

    Bytef * outBuf = (Bytef *)malloc((size_t)rawLen);
    memset(outBuf, 0, (size_t)rawLen);
    uLongf outLen = (uLongf)rawLen;
    uncompress(outBuf, &outLen, compBuf, (uLong)defStrm.total_out);

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzf, compBuf, (unsigned int)defStrm.total_out);
    gzclose(gzf);

    gzFile gzfd = gzdopen(1, "wb");
    gzwrite(gzfd, outBuf, (unsigned int)outLen);
    gzclose(gzfd);

    // step 4: Validate & Cleanup
    inflateBackEnd(&infStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}