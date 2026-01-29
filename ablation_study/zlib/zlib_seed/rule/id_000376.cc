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
//<ID> 376
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
    unsigned char input[256];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum_orig = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize inflate and configure stream
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)inputLen;

    // step 3: Operate: inflate, validate with adler32, and exercise inflateBack
    inflate(&strm, 0);
    uLong checksum_out = adler32(0L, (const Bytef *)outBuf, (uInt)inputLen);
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    backStrm.next_in = compBuf;
    backStrm.avail_in = (uInt)compDestLen;
    backStrm.next_out = outBuf;
    backStrm.avail_out = (uInt)inputLen;
    inflateBack(&backStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&backStrm);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    inflateEnd(&strm);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}