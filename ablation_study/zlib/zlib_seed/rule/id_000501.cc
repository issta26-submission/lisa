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
//<ID> 501
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (create sample data and perform a first-stage compression)
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    uLong compLenU = (uLong)compLen;

    // step 2: Initialize & configure deflate stream
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = compBuf;
    defStrm.avail_in = (uInt)compLenU;
    uLong outBound = compressBound(compLenU);
    Bytef * outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    defStrm.next_out = outBuf;
    defStrm.avail_out = (uInt)outBound;

    // step 3: Operate & validate (further compress the already compressed data and write original data to a gz file)
    deflate(&defStrm, 4);
    uLong secondStageLen = defStrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, input, (unsigned int)inputLen);
    gzclose(gz);
    (void)secondStageLen;

    // step 4: Cleanup
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}