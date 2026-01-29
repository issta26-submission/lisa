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
//<ID> 251
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
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Setup / Configure
    Bytef presetDict[32];
    memset(presetDict, 'D', sizeof(presetDict));
    deflateSetDictionary(&defStrm, presetDict, (uInt)sizeof(presetDict));
    uLong bound = deflateBound(&defStrm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzFile gz_fd = gzdopen(1, "wb");

    // step 3: Core operations
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    gzwrite(gz, compBuf, (unsigned int)defStrm.total_out);
    gzwrite(gz_fd, compBuf, (unsigned int)defStrm.total_out);
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf outLen = (uLongf)inputLen;
    uncompress(outBuf, &outLen, compBuf, (uLong)defStrm.total_out);
    inflateUndermine(&infStrm, 1);
    inflateBackEnd(&infStrm);

    // step 4: Validate & Cleanup
    deflateEnd(&defStrm);
    gzclose(gz);
    gzclose(gz_fd);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}