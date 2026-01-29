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
//<ID> 239
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
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong init_adler = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong init_crc = crc32(0L, (const Bytef *)input, (uInt)inputLen);

    // step 2: Setup
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&dst, &src);
    uLong bound_src = deflateBound(&src, inputLen);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)bound_src);
    memset(compBuf1, 0, (size_t)bound_src);

    // step 3: Core operations
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)inputLen;
    src.next_out = compBuf1;
    src.avail_out = (uInt)bound_src;
    deflate(&src, 0);

    uLong bound_dst = deflateBound(&dst, inputLen);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)bound_dst);
    memset(compBuf2, 0, (size_t)bound_dst);
    dst.next_in = (Bytef *)input;
    dst.avail_in = (uInt)inputLen;
    dst.next_out = compBuf2;
    dst.avail_out = (uInt)bound_dst;
    deflate(&dst, 0);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf1, (unsigned int)src.total_out);
    gzwrite(gz, compBuf2, (unsigned int)dst.total_out);
    gzclose(gz);

    uLong combined_adler = adler32(init_adler, compBuf1, (uInt)src.total_out);
    uLong combined_crc = crc32(init_crc, compBuf2, (uInt)dst.total_out);

    // step 4: Validate & Cleanup
    deflateEnd(&src);
    deflateEnd(&dst);
    free(compBuf1);
    free(compBuf2);

    // API sequence test completed successfully
    (void)combined_adler;
    (void)combined_crc;
    return 66;
}