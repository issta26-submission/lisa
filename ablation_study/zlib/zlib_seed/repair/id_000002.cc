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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const char *version = zlibVersion();
    uLong inputLen = 64;
    Bytef *input = (Bytef *)malloc((size_t)inputLen);
    uLong outMax = compressBound(inputLen);
    Bytef *output = (Bytef *)malloc((size_t)outMax);
    gzFile gz = (gzFile)0;
    off_t gz_pos = (off_t)0;
    unsigned long codes_used = 0;
    // step 2: Setup / Initialize
    memset(input, 'A', (size_t)inputLen);
    deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = output;
    strm.avail_out = (uInt)outMax;
    // step 3: Operate / Validate
    deflate(&strm, 4); // 4 == Z_FINISH
    uLong compressedLen = (uLong)strm.total_out;
    gz = gzopen("test_zlib_api_sequence.tmp", "wb");
    gzbuffer(gz, 4096u);
    gzwrite(gz, output, (unsigned int)compressedLen);
    gz_pos = gzoffset(gz);
    codes_used = inflateCodesUsed(&strm);
    // step 4: Cleanup
    deflateEnd(&strm);
    gzclose(gz);
    free(input);
    free(output);
    // API sequence test completed successfully
    (void)gz_pos;
    (void)codes_used;
    return 66;
}