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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize input and compression buffers
    z_stream backStrm;
    z_stream copyStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    memset(&copyStrm, 0, sizeof(copyStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize inflateBack and copy its state
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&copyStrm, &backStrm);

    // step 3: Configure streams and perform inflateBack using the original stream
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    backStrm.next_in = compBuf;
    backStrm.avail_in = (uInt)compLen;
    backStrm.next_out = outBuf;
    backStrm.avail_out = (uInt)inputLen;
    zlibCompileFlags();
    inflateBack(&backStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&backStrm);
    inflateBackEnd(&copyStrm);

    // step 4: Write compressed data to gz file, flush, cleanup and return
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzflush(gz, (int)((zlibCompileFlags() >> 8) & 3));
    gzclose(gz);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}