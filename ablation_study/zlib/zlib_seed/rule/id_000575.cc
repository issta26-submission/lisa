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
//<ID> 575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char * ver = zlibVersion();
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the input and compute CRCs
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    uLong crc_input = crc32(0, (const Bytef *)input, (uInt)inputLen);
    uLong crc_comp = crc32(0, compBuf, (uInt)compLen);
    off_t input_off = (off_t)inputLen;
    uLong crc_combined = crc32_combine(crc_input, crc_comp, input_off);

    // step 3: Persist compressed data to a gz file, query position, and inspect compile flags
    gzFile gz = gzopen64("test_zlib_api.gz", "wb");
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    off64_t pos = gztell64(gz);
    gzclose(gz);
    uLong flags = zlibCompileFlags();
    uLong combined_with_flags = crc_combined ^ flags;
    (void)pos;
    (void)combined_with_flags;
    (void)ver;

    // step 4: Cleanup
    free(compBuf);

    return 66; // API sequence test completed successfully
}