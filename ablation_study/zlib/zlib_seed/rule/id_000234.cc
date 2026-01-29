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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream dest;
    memset(&src, 0, sizeof(src));
    memset(&dest, 0, sizeof(dest));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum_adler = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong checksum_crc = crc32(0L, (const Bytef *)input, (uInt)inputLen);

    // step 2: Setup
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&src, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)inputLen;
    src.next_out = compBuf;
    src.avail_out = (uInt)bound;
    deflate(&src, 0);
    deflateCopy(&dest, &src);

    // step 3: Core operations
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, compBuf, (unsigned int)src.total_out);
    gzclose(gf);

    // step 4: Validate & Cleanup
    deflateEnd(&dest);
    deflateEnd(&src);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}