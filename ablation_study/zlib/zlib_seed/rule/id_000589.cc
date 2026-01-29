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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    const char * ver = zlibVersion();
    Bytef * input = (Bytef *)malloc((size_t)128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;

    // step 2: Initialize streams and allocate output buffer
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&defStrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    unsigned char * window = (unsigned char *)malloc(1);
    memset(window, 0, 1);
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Operate: compress, compute checksum, adjust inflate behavior, finalize back-end
    deflate(&defStrm, 0);
    uLong chk = adler32(0L, input, (uInt)inputLen);
    defStrm.adler = chk;
    inflateUndermine(&infStrm, 1);
    inflateBackEnd(&backStrm);

    // step 4: Cleanup
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    free(input);
    free(compBuf);
    free(window);
    // API sequence test completed successfully
    return 66;
}