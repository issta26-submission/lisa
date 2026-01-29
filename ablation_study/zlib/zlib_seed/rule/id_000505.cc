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
//<ID> 505
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 2: Initialize & Configure
    Bytef * defOut = (Bytef *)malloc((size_t)bound);
    memset(defOut, 0, (size_t)bound);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (unsigned int)inputLen;
    defStrm.next_out = defOut;
    defStrm.avail_out = (unsigned int)bound;

    // step 3: Operate & Validate
    deflate(&defStrm, 0);
    uLong defProduced = (uLong)((uLong)bound - (uLong)defStrm.avail_out);
    (void)defProduced;
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&defStrm);
    free(compBuf);
    free(defOut);

    return 66; // API sequence test completed successfully
}