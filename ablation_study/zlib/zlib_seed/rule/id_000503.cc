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
//<ID> 503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (create sample data and produce a first-stage compressed buffer)
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound1 = compressBound(inputLen);
    Bytef * compBuf1 = (Bytef *)malloc((size_t)bound1);
    memset(compBuf1, 0, (size_t)bound1);
    uLongf compLen1 = (uLongf)bound1;
    compress(compBuf1, &compLen1, (const Bytef *)input, inputLen);

    // step 2: Initialize & Configure deflate stream to further compress the already compressed data
    uLong bound2 = compressBound((uLong)compLen1);
    Bytef * defOutBuf = (Bytef *)malloc((size_t)bound2);
    memset(defOutBuf, 0, (size_t)bound2);
    memset(&defStrm, 0, sizeof(defStrm));
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = compBuf1;
    defStrm.avail_in = (uInt)compLen1;
    defStrm.next_out = defOutBuf;
    defStrm.avail_out = (uInt)bound2;
    deflate(&defStrm, Z_FINISH);
    uLong defOutLen = (uLong)((uLong)bound2 - (uLong)defStrm.avail_out);

    // step 3: Operate & Validate — write the deflated buffer to a gzip file
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, defOutBuf, (unsigned int)defOutLen);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&defStrm);
    free(compBuf1);
    free(defOutBuf);

    return 66; // API sequence test completed successfully
}