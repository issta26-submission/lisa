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
//<ID> 572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char * ver = zlibVersion();
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress input, compute CRCs and get compile flags
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    uLong crc_input = crc32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong crc_comp = crc32(0UL, compBuf, (uInt)compLen);
    uLong compile_flags = zlibCompileFlags();
    (void)ver;
    (void)compile_flags;

    // step 3: Open a gz file, write compressed data, and query position
    gzFile gz = gzopen64("temp_zlib_test.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t pos = gztell64(gz);
    (void)pos;

    // step 4: Combine CRCs, cleanup and finish
    uLong combined_crc = crc32_combine(crc_input, crc_comp, (off_t)inputLen);
    (void)combined_crc;
    gzclose(gz);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}