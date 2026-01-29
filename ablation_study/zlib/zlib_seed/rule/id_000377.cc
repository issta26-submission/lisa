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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Prepare input, compress, checksum, and write gz
    z_stream backStrm;
    z_stream strm;
    memset(&backStrm, 0, sizeof(backStrm));
    memset(&strm, 0, sizeof(strm));
    unsigned char input[128];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong origAdler = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzflush(gz, 0);
    gzclose(gz);

    // step 2: Initialize and run inflateBack on compressed data
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf1 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf1, 0, (size_t)inputLen);
    backStrm.next_in = compBuf;
    backStrm.avail_in = (uInt)compDestLen;
    backStrm.next_out = outBuf1;
    backStrm.avail_out = (uInt)inputLen;
    inflateBack(&backStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&backStrm);

    // step 3: Initialize and run inflate (stream) on compressed data and checksum result
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf2;
    strm.avail_out = (uInt)inputLen;
    inflate(&strm, 0);
    uLong decompressedAdler = adler32(0UL, outBuf2, (uInt)inputLen);
    inflateEnd(&strm);

    // step 4: Cleanup resources and return
    free(compBuf);
    free(outBuf1);
    free(outBuf2);
    return 66; // API sequence test completed successfully
}