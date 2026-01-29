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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream dst;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    unsigned char input[512];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = compBuf;
    src.avail_in = (uInt)compLen;
    deflateCopy(&dst, &src);

    // step 3: Core operations
    uLong outSize = compLen + 1024;
    Bytef *outBuf = (Bytef *)malloc((size_t)outSize);
    memset(outBuf, 0, (size_t)outSize);
    dst.next_in = compBuf;
    dst.avail_in = (uInt)compLen;
    dst.next_out = outBuf;
    dst.avail_out = (uInt)outSize;
    deflate(&dst, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)outBuf, (unsigned int)dst.total_out);
    deflateEnd(&dst);
    deflateEnd(&src);
    gzclose(gz);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}