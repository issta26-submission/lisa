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
//<ID> 182
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
    unsigned char pad_out[1024];
    memset(input, 'A', sizeof(input));
    memset(pad_out, 0, sizeof(pad_out));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup
    uLong checksum = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&def_copy, &def_src);

    // step 3: Core operations
    uLong recompressBound = compressBound(compLen);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_copy.next_in = compBuf;
    def_copy.avail_in = (uInt)compLen;
    def_copy.next_out = recompBuf;
    def_copy.avail_out = (uInt)recompressBound;
    deflate(&def_copy, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_copy.total_out);
    gzclose(gz);
    deflateEnd(&def_copy);
    deflateEnd(&def_src);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}