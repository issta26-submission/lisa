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
//<ID> 360
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
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 2: Configure inflateBack & make a copy of the stream
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)inputLen;
    inflateCopy(&copyStrm, &strm);

    // step 3: Operate (use inflateBack on the copied stream) and finish back-inflate
    int rc = inflateBack(&copyStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    (void)rc;
    inflateBackEnd(&copyStrm);

    // step 4: Validate & Cleanup (write compressed data to gz file, flush, close, free, end original)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzflush(gz, 2);
    gzclose(gz);
    inflateBackEnd(&strm);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}