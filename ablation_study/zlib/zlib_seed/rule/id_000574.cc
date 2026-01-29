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
//<ID> 574
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    Bytef input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    uLong crc1 = crc32(0UL, input, (uInt)(inputLen / 2));
    uLong crc2 = crc32(0UL, input + (inputLen / 2), (uInt)(inputLen - (inputLen / 2)));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(inputLen - (inputLen / 2)));

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, input, inputLen);

    // step 3: Write compressed data to a gz file and get its 64-bit position
    gzFile gf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb+");
    unsigned int writeLen = (unsigned int)(compLen - (uLongf)((combined_crc & 0xFFu) % ((uLongf)compLen + 1u)));
    gzwrite(gf, compBuf, writeLen);
    gzflush(gf, 0);
    off64_t pos = gztell64(gf);

    // step 4: Validate, cleanup & gather build info
    uLong flags = zlibCompileFlags();
    (void)pos;
    (void)flags;
    gzclose(gf);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}