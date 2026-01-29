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
//<ID> 585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    const char *ver = zlibVersion();
    uLong inputLen = (uLong)128;
    Bytef *input = (Bytef *)malloc((size_t)inputLen);
    memset(input, 'Z', (size_t)inputLen);
    uLong checksum_before = adler32(0L, input, (uInt)inputLen);
    defStrm.zalloc = NULL;
    defStrm.zfree = NULL;
    defStrm.opaque = NULL;
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure streams and buffers
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflateParams(&defStrm, 6, 0);

    // step 3: Operate (compress) and Validate (checksums & inflate config)
    deflate(&defStrm, 4);
    deflateEnd(&defStrm);
    uLong compressedBytes = defStrm.total_out;
    uLong checksum_after = adler32(0L, compBuf, (uInt)compressedBytes);
    inflateUndermine(&infStrm, 1);
    inflateBackEnd(&infStrm);
    inflateEnd(&infStrm);

    // step 4: Cleanup
    free(input);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}