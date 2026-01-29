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
//<ID> 237
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
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum_adler = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong checksum_crc = crc32(0L, (const Bytef *)input, (uInt)inputLen);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, input, (unsigned int)inputLen);
    gzclose(gf);

    // step 2: Setup
    deflateInit_(&src_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound1 = deflateBound(&src_strm, inputLen);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)compBound1);
    memset(compBuf1, 0, (size_t)compBound1);
    src_strm.next_in = (Bytef *)input;
    src_strm.avail_in = (uInt)inputLen;
    src_strm.next_out = compBuf1;
    src_strm.avail_out = (uInt)compBound1;

    // step 3: Core operations
    deflate(&src_strm, 0);
    deflateCopy(&dst_strm, &src_strm);
    uLong compBound2 = deflateBound(&dst_strm, (uLong)src_strm.total_in);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound2);
    memset(compBuf2, 0, (size_t)compBound2);
    dst_strm.next_in = src_strm.next_in;
    dst_strm.avail_in = src_strm.avail_in;
    dst_strm.next_out = compBuf2;
    dst_strm.avail_out = (uInt)compBound2;
    deflate(&dst_strm, 0);

    // step 4: Validate & Cleanup
    deflateEnd(&src_strm);
    deflateEnd(&dst_strm);
    free(compBuf1);
    free(compBuf2);
    (void)checksum_adler;
    (void)checksum_crc;
    // API sequence test completed successfully
    return 66;
}