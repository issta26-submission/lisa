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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare data and compressed buffer
    z_stream strmInfl;
    z_stream strmBack;
    memset(&strmInfl, 0, sizeof(strmInfl));
    memset(&strmBack, 0, sizeof(strmBack));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize inflate and inflateBack with windows and output buffers
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateInit_(&strmInfl, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&strmBack, 15, window, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outInfl = (Bytef *)malloc((size_t)inputLen);
    Bytef * outBack = (Bytef *)malloc((size_t)inputLen);
    memset(outInfl, 0, (size_t)inputLen);
    memset(outBack, 0, (size_t)inputLen);
    strmInfl.next_in = compBuf;
    strmInfl.avail_in = (uInt)compDestLen;
    strmInfl.next_out = outInfl;
    strmInfl.avail_out = (uInt)inputLen;
    strmBack.next_in = compBuf;
    strmBack.avail_in = (uInt)compDestLen;
    strmBack.next_out = outBack;
    strmBack.avail_out = (uInt)inputLen;

    // step 3: Operate - inflate, exercise undermine API, and perform inflateBack
    inflate(&strmInfl, 0);
    inflateUndermine(&strmInfl, 1);
    inflateBack(&strmBack, (in_func)0, (void *)0, (out_func)0, (void *)0);

    // step 4: Validate-like cleanup: write compressed data to gz, close, and cleanup inflate states
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    inflateEnd(&strmInfl);
    inflateBackEnd(&strmBack);
    free(compBuf);
    free(outInfl);
    free(outBack);
    return 66;
    // API sequence test completed successfully
}