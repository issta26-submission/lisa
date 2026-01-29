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
//<ID> 183
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
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)bound);
    uLong compLen1 = bound;
    compress2(compBuf1, &compLen1, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize deflater and perform a deflate, then copy)
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    Bytef *compBuf2 = (Bytef *)malloc((size_t)bound);
    memset(compBuf2, 0, (size_t)bound);
    def_src.next_in = (Bytef *)input;
    def_src.avail_in = (uInt)inputLen;
    def_src.next_out = compBuf2;
    def_src.avail_out = (uInt)bound;
    deflate(&def_src, 0);
    deflateCopy(&def_copy, &def_src);

    // step 3: Core operations (use adler32, feed previously compress2 output into the copied deflater and deflate)
    uLong checksum = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    (void)checksum;
    Bytef *finalBuf = (Bytef *)malloc((size_t)bound);
    memset(finalBuf, 0, (size_t)bound);
    def_copy.next_in = compBuf1;
    def_copy.avail_in = (uInt)compLen1;
    def_copy.next_out = finalBuf;
    def_copy.avail_out = (uInt)bound;
    deflate(&def_copy, 0);

    // step 4: Validate & Cleanup (write final buffer to a gz file, cleanup resources)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)finalBuf, (unsigned int)def_copy.total_out);
    gzclose(gz);
    deflateEnd(&def_copy);
    deflateEnd(&def_src);
    free(compBuf1);
    free(compBuf2);
    free(finalBuf);
    // API sequence test completed successfully
    return 66;
}