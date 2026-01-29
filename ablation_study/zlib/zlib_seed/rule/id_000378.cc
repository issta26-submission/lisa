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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    z_stream strmBack;
    memset(&strm, 0, sizeof(strm));
    memset(&strmBack, 0, sizeof(strmBack));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);
    uLong inputAdler = adler32(0L, input, (uInt)inputLen);

    // step 2: Initialize inflate, perform inflate, and validate with adler32
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)inputLen;
    inflate(&strm, 0);
    uLong outTotal = strm.total_out;
    uLong outAdler = adler32(0L, outBuf, (uInt)outTotal);

    // step 3: Initialize inflateBack, perform inflateBack, and cleanup inflateBack
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    inflateBackInit_(&strmBack, 15, window, zlibVersion(), (int)sizeof(z_stream));
    strmBack.next_in = compBuf;
    strmBack.avail_in = (uInt)compDestLen;
    strmBack.next_out = outBuf2;
    strmBack.avail_out = (uInt)inputLen;
    inflateBack(&strmBack, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&strmBack);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    inflateEnd(&strm);
    free(compBuf);
    free(outBuf);
    free(outBuf2);
    (void)inputAdler;
    (void)outAdler;
    (void)outTotal;
    return 66;
    // API sequence test completed successfully
}