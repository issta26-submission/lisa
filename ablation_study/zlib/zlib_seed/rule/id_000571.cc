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
//<ID> 571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong inputLen = 256;
    Bytef * input = (Bytef *)malloc((size_t)inputLen);
    memset(input, 'Z', (size_t)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    gzFile gz = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    uLong crc1 = crc32(0UL, input, (uInt)(inputLen / 2));
    uLong crc2 = crc32(0UL, input + (inputLen / 2), (uInt)(inputLen - (inputLen / 2)));

    // step 2: Compress the input
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 3: Write compressed data, query position, combine CRCs, query compile flags
    gzwrite(gz, compBuf, (unsigned)compLen);
    off64_t pos = gztell64(gz);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)inputLen);
    uLong flags = zlibCompileFlags();
    (void)pos;
    (void)combined_crc;
    (void)flags;

    // step 4: Cleanup
    gzclose(gz);
    free(compBuf);
    free(input);
    return 66; // API sequence test completed successfully
}