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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and compress
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf compLen = (uLongf)bound;
    const char *ver = zlibVersion();
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 2: Initialize inflate, configure stream, use inflateSync and inflate
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateSync(&infStrm);
    inflate(&infStrm, Z_FINISH);
    inflateEnd(&infStrm);

    // step 3: Initialize inflateBack and immediately end it
    unsigned char *window = (unsigned char *)malloc(1u << 15);
    memset(window, 0, 1u << 15);
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&backStrm);
    free(window);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}