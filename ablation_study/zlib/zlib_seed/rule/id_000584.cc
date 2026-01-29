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
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    const char *ver = zlibVersion();
    Bytef *input = (Bytef *)malloc((size_t)256);
    memset(input, 'X', 256);
    uLong inputLen = (uLong)256;
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    unsigned char *window = (unsigned char *)malloc((size_t)(1u << 15));
    memset(window, 0, (size_t)(1u << 15));

    // step 2: Initialize & configure deflate
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;

    // step 3: Operate: compress, validate, and initialize inflate-back then undermine
    deflate(&defStrm, 4); /* 4 == Z_FINISH */
    uLong compressedLen = defStrm.total_out;
    uLong checksum = adler32(0L, input, (uInt)inputLen);
    (void)compressedLen;
    (void)checksum;
    inflateBackInit_(&infStrm, 15, window, ver, (int)sizeof(z_stream));
    inflateUndermine(&infStrm, 1);
    inflateBackEnd(&infStrm);

    // step 4: Cleanup
    deflateEnd(&defStrm);
    free(input);
    free(compBuf);
    free(window);

    // API sequence test completed successfully
    return 66;
}