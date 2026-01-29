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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    z_stream copyStrm;
    memset(&strm, 0, sizeof(strm));
    memset(&copyStrm, 0, sizeof(copyStrm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong flags = zlibCompileFlags();
    int level = (int)((flags & 3) + 6);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, level);

    // step 2: Initialize inflateBack and make a copy
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&copyStrm, &strm);

    // step 3: Configure streams and call inflateBack, then cleanup inflate state
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)inputLen;
    copyStrm.next_in = compBuf;
    copyStrm.avail_in = (uInt)compDestLen;
    copyStrm.next_out = outBuf;
    copyStrm.avail_out = (uInt)inputLen;
    inflateBack(&copyStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&copyStrm);
    inflateBackEnd(&strm);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzflush(gz, 0);
    gzclose(gz);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}