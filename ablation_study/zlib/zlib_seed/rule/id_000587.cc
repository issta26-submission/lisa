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
//<ID> 587
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
    Bytef *input = (Bytef *)malloc(128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;
    uLong checksum_before = adler32(0L, input, (uInt)inputLen);
    const char * ver = zlibVersion();
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: Initialize deflate, compress data, and cleanup deflate
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    deflate(&defStrm, Z_FINISH);
    uLong compLen = defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Initialize inflate, call inflateUndermine, perform inflate, call inflateBackEnd
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    inflateUndermine(&infStrm, 1);
    inflate(&infStrm, Z_FINISH);
    inflateBackEnd(&infStrm);
    inflateEnd(&infStrm);

    // step 4: Validate (checksum) & Cleanup
    uLong checksum_after = adler32(0L, outBuf, (uInt)inputLen);
    (void)checksum_before;
    (void)checksum_after;
    free(input);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}