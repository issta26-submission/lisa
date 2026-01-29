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
//<ID> 238
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
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong init_adler = adler32(0, (const Bytef *)input, (uInt)inputLen);
    uLong init_crc = crc32(0, (const Bytef *)input, (uInt)inputLen);

    // step 2: Setup
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound = deflateBound(&def_src, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_src.next_in = (Bytef *)input;
    def_src.avail_in = (uInt)inputLen;
    def_src.next_out = compBuf;
    def_src.avail_out = (uInt)compBound;

    // step 3: Core operations
    deflate(&def_src, 0);
    deflateCopy(&def_copy, &def_src);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)def_src.total_out);
    gzclose(gz);
    uLong comp_adler = adler32(0, compBuf, (uInt)def_src.total_out);
    uLong comp_crc = crc32(0, compBuf, (uInt)def_src.total_out);

    // step 4: Cleanup
    deflateEnd(&def_copy);
    deflateEnd(&def_src);
    free(compBuf);
    // API sequence test completed successfully
    return 66;
}