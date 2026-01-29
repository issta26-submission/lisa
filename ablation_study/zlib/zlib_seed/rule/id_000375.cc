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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    z_stream backStrm;
    memset(&strm, 0, sizeof(strm));
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLongf compDestLen = 0;
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);
    uLong orig_adler = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Initialize inflateBack and inflate
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Configure streams & Operate
    Bytef * outBuf1 = (Bytef *)malloc((size_t)inputLen);
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf1, 0, (size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    backStrm.next_in = compBuf;
    backStrm.avail_in = (uInt)compDestLen;
    backStrm.next_out = outBuf1;
    backStrm.avail_out = (uInt)inputLen;
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf2;
    strm.avail_out = (uInt)inputLen;
    inflateBack(&backStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflate(&strm, 0);

    // step 4: Validate & Cleanup
    uLong out_adler = adler32(0UL, (const Bytef *)outBuf2, (uInt)inputLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    inflateBackEnd(&backStrm);
    inflateEnd(&strm);
    free(compBuf);
    free(outBuf1);
    free(outBuf2);
    // API sequence test completed successfully
    return 66;
}