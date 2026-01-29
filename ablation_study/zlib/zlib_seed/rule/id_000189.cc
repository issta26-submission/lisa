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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream def_copy;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&def_copy, 0, sizeof(def_copy));
    unsigned char input[512];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    (void)checksum;

    // step 2: Setup (initialize deflater and make a copy)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&def_copy, &def_strm);

    // step 3: Core operations (use both deflater and its copy to compress the same data)
    uLong defOutSize1 = compressBound(inputLen);
    uLong defOutSize2 = compressBound(inputLen);
    Bytef *defOutBuf1 = (Bytef *)malloc((size_t)defOutSize1);
    Bytef *defOutBuf2 = (Bytef *)malloc((size_t)defOutSize2);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = defOutBuf1;
    def_strm.avail_out = (uInt)defOutSize1;
    def_copy.next_in = (Bytef *)input;
    def_copy.avail_in = (uInt)inputLen;
    def_copy.next_out = defOutBuf2;
    def_copy.avail_out = (uInt)defOutSize2;
    deflate(&def_strm, 0);
    deflate(&def_copy, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, defOutBuf1, (unsigned int)def_strm.total_out);
    gzwrite(gz, defOutBuf2, (unsigned int)def_copy.total_out);
    gzclose(gz);
    deflateEnd(&def_strm);
    deflateEnd(&def_copy);
    free(compBuf);
    free(defOutBuf1);
    free(defOutBuf2);

    // API sequence test completed successfully
    return 66;
}