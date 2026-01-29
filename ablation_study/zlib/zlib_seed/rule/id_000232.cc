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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src_strm;
    z_stream copy_strm;
    memset(&src_strm, 0, sizeof(src_strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong ad1 = adler32(1UL, input, (uInt)inputLen);
    uLong crc1 = crc32(0UL, input, (uInt)inputLen);

    // step 2: Setup
    deflateInit_(&src_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound = deflateBound(&src_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    src_strm.next_in = (Bytef *)input;
    src_strm.avail_in = (uInt)inputLen;
    src_strm.next_out = compBuf;
    src_strm.avail_out = (uInt)compBound;

    // step 3: Core operations
    deflate(&src_strm, 0);
    deflateCopy(&copy_strm, &src_strm);
    gzFile gf = gzopen("test_zlib_api.gz", "wb");
    gzwrite(gf, input, (unsigned int)inputLen);
    gzclose(gf);
    uLong compProduced = src_strm.total_out;
    uLong ad2 = adler32(ad1, compBuf, (uInt)compProduced);
    uLong crc2 = crc32(crc1, compBuf, (uInt)compProduced);

    // step 4: Validate & Cleanup
    deflateEnd(&copy_strm);
    deflateEnd(&src_strm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}