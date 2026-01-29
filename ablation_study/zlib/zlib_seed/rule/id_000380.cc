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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Prepare input and compressed buffer
    z_stream strmInfl;
    z_stream strmBack;
    memset(&strmInfl, 0, sizeof(strmInfl));
    memset(&strmBack, 0, sizeof(strmBack));
    unsigned char input[160];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize inflate and inflateBack, configure streams and output buffers
    inflateInit_(&strmInfl, zlibVersion(), (int)sizeof(z_stream));
    unsigned char window[32];
    memset(window, 0, sizeof(window));
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

    // step 3: Operate - inflate and inflateBack, then validate with inflateUndermine
    inflate(&strmInfl, 0);
    inflateBack(&strmBack, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateUndermine(&strmInfl, 1);

    // step 4: Cleanup - finalize inflate states, write compressed data to gz file, free memory
    inflateEnd(&strmInfl);
    inflateBackEnd(&strmBack);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    free(compBuf);
    free(outInfl);
    free(outBack);
    return 66;
    // API sequence test completed successfully
}