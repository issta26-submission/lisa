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
//<ID> 187
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
    unsigned char out_buf[512];
    memset(input, 'B', sizeof(input));
    memset(out_buf, 0, sizeof(out_buf));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong checksum = adler32(0UL, compBuf, (uInt)compLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 2: Setup
    deflateInit_(&src_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&copy_strm, &src_strm);

    // step 3: Core operations
    copy_strm.next_in = (Bytef *)input;
    copy_strm.avail_in = (uInt)inputLen;
    copy_strm.next_out = (Bytef *)out_buf;
    copy_strm.avail_out = (uInt)sizeof(out_buf);
    deflate(&copy_strm, 0);

    // step 4: Validate & Cleanup
    gzwrite(gz, (voidpc)out_buf, (unsigned int)copy_strm.total_out);
    deflateEnd(&copy_strm);
    deflateEnd(&src_strm);
    gzclose(gz);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}