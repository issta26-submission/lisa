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
//<ID> 506
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (create sample data and compress it)
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound1 = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound1);
    memset(compBuf, 0, (size_t)compBound1);
    uLongf compLen = (uLongf)compBound1;
    int compRes = compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    (void)compRes;

    // step 2: Initialize & Configure deflate stream
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = compBuf;
    defStrm.avail_in = (uInt)compLen;
    uLong defOutBound = compressBound((uLong)compLen);
    Bytef * defOut = (Bytef *)malloc((size_t)defOutBound);
    memset(defOut, 0, (size_t)defOutBound);
    defStrm.next_out = defOut;
    defStrm.avail_out = (uInt)defOutBound;

    // step 3: Operate & Validate (deflate the compressed data and write original data to gz file)
    int defRes = deflate(&defStrm, Z_FINISH);
    (void)defRes;
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int wrote = gzwrite(gz, input, (unsigned int)inputLen);
    (void)wrote;

    // step 4: Cleanup
    deflateEnd(&defStrm);
    gzclose(gz);
    free(compBuf);
    free(defOut);

    return 66; // API sequence test completed successfully
}