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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers, compress source data
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
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize inflate, provide compressed data, sync and inflate
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateSync(&infStrm);
    inflate(&infStrm, Z_FINISH);
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);

    // step 3: Initialize inflateBack (alternate inflate path) and immediately clean it up
    unsigned char windowBuf[32768];
    memset(windowBuf, 0, sizeof(windowBuf));
    inflateBackInit_(&backStrm, 15, windowBuf, zlibVersion(), (int)sizeof(z_stream));
    inflateBackEnd(&backStrm);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf);

    return 66;
    // API sequence test completed successfully
}