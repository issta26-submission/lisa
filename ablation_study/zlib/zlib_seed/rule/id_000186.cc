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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_src;
    z_stream def_copy;
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_copy, 0, sizeof(def_copy));
    unsigned char input[512];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize deflater and copy it, compute checksum)
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&def_copy, &def_src);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 3: Core operations (compress the original input using the copied deflater)
    uLong outBound = compressBound(inputLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    def_copy.next_in = (Bytef *)input;
    def_copy.avail_in = (uInt)inputLen;
    def_copy.next_out = outBuf;
    def_copy.avail_out = (uInt)outBound;
    deflate(&def_copy, 0);

    // step 4: Validate & Cleanup (write compressed buffers and checksum to gz, then cleanup)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    gzwrite(gz, (voidpc)outBuf, (unsigned int)def_copy.total_out);
    unsigned char chkbuf[8];
    chkbuf[0] = (unsigned char)(checksum & 0xFF);
    chkbuf[1] = (unsigned char)((checksum >> 8) & 0xFF);
    chkbuf[2] = (unsigned char)((checksum >> 16) & 0xFF);
    chkbuf[3] = (unsigned char)((checksum >> 24) & 0xFF);
    gzwrite(gz, (voidpc)chkbuf, 4);
    deflateEnd(&def_copy);
    deflateEnd(&def_src);
    gzclose(gz);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}