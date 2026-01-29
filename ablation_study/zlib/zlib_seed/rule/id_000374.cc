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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialization
    z_stream strm;
    z_stream backStrm;
    memset(&strm, 0, sizeof(strm));
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Compress input to produce a compressed buffer
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 3: Initialize inflate and inflateBack, configure streams and run operations
    Bytef * outBuf1 = (Bytef *)malloc((size_t)inputLen);
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf1, 0, (size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf1;
    strm.avail_out = (uInt)inputLen;
    inflate(&strm, 0);
    backStrm.next_in = compBuf;
    backStrm.avail_in = (uInt)compDestLen;
    backStrm.next_out = outBuf2;
    backStrm.avail_out = (uInt)inputLen;
    inflateBack(&backStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);

    // step 4: Write compressed data to gz file and Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    inflateEnd(&strm);
    inflateBackEnd(&backStrm);
    free(compBuf);
    free(outBuf1);
    free(outBuf2);
    (void)checksum;
    return 66; // API sequence test completed successfully
}