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
//<ID> 370
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 2: Configure inflate and prepare output buffer
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)inputLen;

    // step 3: Operate - call inflate, compute checksum, then use inflateBack on a separate stream
    inflate(&strm, 0);
    uLong checksum = adler32(0UL, outBuf, (uInt)inputLen);
    (void)checksum;
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&strmBack, 15, window, zlibVersion(), (int)sizeof(z_stream));
    strmBack.next_in = compBuf;
    strmBack.avail_in = (uInt)compDestLen;
    strmBack.next_out = outBuf;
    strmBack.avail_out = (uInt)inputLen;
    inflateBack(&strmBack, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&strmBack);

    // step 4: Validate writing compressed data to a gz file and Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    inflateEnd(&strm);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}