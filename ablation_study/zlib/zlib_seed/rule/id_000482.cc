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
//<ID> 482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialization
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    deflateInit2_(&defStrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    inflateInit2_(&infStrm, 15, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Operate (compress to produce a source buffer)
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 3: Operate (uncompress2 using the produced compressed buffer)
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf destLen = (uLongf)inputLen;
    uLong srcLen = (uLong)compLen;
    uncompress2(outBuf, &destLen, compBuf, &srcLen);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0UL, outBuf, (uInt)destLen);
    (void)checksum;
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}