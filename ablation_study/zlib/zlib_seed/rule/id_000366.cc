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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    z_stream copy;
    memset(&strm, 0, sizeof(strm));
    memset(&copy, 0, sizeof(copy));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize inflateBack and configure streams
    unsigned char window[64];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)inputLen;

    // step 3: Use zlibCompileFlags, copy the stream, and run inflateBack
    uLong flags = zlibCompileFlags();
    (void)flags;
    inflateCopy(&copy, &strm);
    inflateBack(&strm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBack(&copy, (in_func)0, (void *)0, (out_func)0, (void *)0);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzflush(gz, 0);
    gzclose(gz);
    inflateBackEnd(&strm);
    inflateBackEnd(&copy);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}