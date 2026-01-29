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
//<ID> 586
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
    memset(&defStrm, 0, (int)sizeof(z_stream));
    memset(&infStrm, 0, (int)sizeof(z_stream));
    const char *ver = zlibVersion();
    Bytef *input = (Bytef *)malloc((size_t)256);
    memset(input, 'A', 256);
    uLong inputLen = (uLong)256;
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Initialize & configure streams
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;

    // step 3: Operate & validate data flow
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong checksum = adler32(0UL, input, (uInt)inputLen);
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.adler = checksum;
    int undRes = inflateUndermine(&infStrm, 1);
    (void)undRes;
    int backRes = inflateBackEnd(&infStrm);
    (void)backRes;

    // step 4: Cleanup
    int e1 = deflateEnd(&defStrm);
    int e2 = inflateEnd(&infStrm);
    (void)e1;
    (void)e2;
    free(input);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}