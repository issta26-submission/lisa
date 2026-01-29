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
//<ID> 529
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (create sample data and allocate buffers)
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Operate (compress the input using compress)
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 3: Initialize & Configure inflate stream, then perform inflate
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateReset2(&infStrm, 15);
    inflate(&infStrm, Z_FINISH);

    // step 4: Validate (read totals) & Cleanup (end inflate and free buffers)
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}