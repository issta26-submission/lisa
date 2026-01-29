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
//<ID> 576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    Bytef *input1 = (Bytef *)malloc((size_t)64);
    memset(input1, 'A', (size_t)64);
    Bytef *input2 = (Bytef *)malloc((size_t)32);
    memset(input2, 'B', (size_t)32);
    uLong len1 = (uLong)64;
    uLong len2 = (uLong)32;
    uLong totalLen = len1 + len2;
    uLong bound = compressBound(totalLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *combined = (Bytef *)malloc((size_t)totalLen);
    memcpy(combined, input1, (size_t)len1);
    memcpy(combined + len1, input2, (size_t)len2);

    // step 2: Compress combined data and write to a gzFile
    compress(compBuf, &compLen, combined, totalLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t position64 = gztell64(gz);

    // step 3: Compute CRCs and combine, query compile flags for validation
    uLong crc_first = crc32(0L, input1, (uInt)len1);
    uLong crc_second = crc32(0L, input2, (uInt)len2);
    uLong combined_crc = crc32_combine(crc_first, crc_second, (off_t)len2);
    uLong compile_flags = zlibCompileFlags();
    (void)position64;
    (void)combined_crc;
    (void)compile_flags;

    // step 4: Cleanup
    gzclose(gz);
    free(input1);
    free(input2);
    free(compBuf);
    free(combined);

    // API sequence test completed successfully
    return 66;
}