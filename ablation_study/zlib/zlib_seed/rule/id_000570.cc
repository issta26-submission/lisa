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
//<ID> 570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the input into compBuf
    int compRes = compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    (void)compRes;

    // step 3: Write compressed data to a gz file, query position, combine CRCs, get compile flags
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int wrote = 0;
    wrote = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    (void)wrote;
    off64_t pos = gztell64(gz);
    uLong combined_crc = crc32_combine(0u, 0u, (off_t)pos);
    uLong flags = zlibCompileFlags();
    (void)combined_crc;
    (void)flags;
    gzflush(gz, 0);

    // step 4: Cleanup
    gzclose(gz);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}