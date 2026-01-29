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
//<ID> 230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src_strm;
    z_stream dst_strm;
    memset(&src_strm, 0, sizeof(src_strm));
    memset(&dst_strm, 0, sizeof(dst_strm));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong input_adler = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong input_crc = crc32(0L, (const Bytef *)input, (uInt)inputLen);

    // step 2: Setup
    deflateInit_(&src_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&dst_strm, &src_strm);
    uLong compBound1 = deflateBound(&src_strm, inputLen);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)compBound1);
    memset(compBuf1, 0, (size_t)compBound1);
    uLong compBound2 = deflateBound(&dst_strm, inputLen);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound2);
    memset(compBuf2, 0, (size_t)compBound2);

    // step 3: Core operations
    src_strm.next_in = (Bytef *)input;
    src_strm.avail_in = (uInt)inputLen;
    src_strm.next_out = compBuf1;
    src_strm.avail_out = (uInt)compBound1;
    deflate(&src_strm, 0);

    dst_strm.next_in = (Bytef *)input;
    dst_strm.avail_in = (uInt)inputLen;
    dst_strm.next_out = compBuf2;
    dst_strm.avail_out = (uInt)compBound2;
    deflate(&dst_strm, 0);

    gzFile gf = gzopen("zlib_api_seq_test.gz", "wb");
    gzwrite(gf, compBuf1, (unsigned int)src_strm.total_out);
    gzwrite(gf, compBuf2, (unsigned int)dst_strm.total_out);
    gzclose(gf);

    uLong comp_adler = adler32(1L, compBuf1, (uInt)src_strm.total_out);
    uLong comp_crc = crc32(0L, compBuf2, (uInt)dst_strm.total_out);

    // step 4: Validate & Cleanup
    deflateEnd(&dst_strm);
    deflateEnd(&src_strm);
    free(compBuf2);
    free(compBuf1);

    // API sequence test completed successfully
    return 66;
}