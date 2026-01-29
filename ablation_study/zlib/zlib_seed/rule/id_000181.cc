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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src_def;
    z_stream dst_def;
    memset(&src_def, 0, sizeof(src_def));
    memset(&dst_def, 0, sizeof(dst_def));
    unsigned char input[512];
    unsigned char temp_out[1024];
    memset(input, 'A', sizeof(input));
    memset(temp_out, 0, sizeof(temp_out));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong checksum = adler32(1UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Setup (initialize deflater and make a copy)
    deflateInit_(&src_def, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&dst_def, &src_def);

    // step 3: Core operations (use the copied deflater to compress the input)
    dst_def.next_in = (Bytef *)input;
    dst_def.avail_in = (uInt)inputLen;
    dst_def.next_out = (Bytef *)temp_out;
    dst_def.avail_out = (uInt)sizeof(temp_out);
    deflate(&dst_def, 0);

    // step 4: Validate & Cleanup (write compressed output to gz, then cleanup)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)temp_out, (unsigned int)dst_def.total_out);
    deflateEnd(&dst_def);
    deflateEnd(&src_def);
    gzclose(gz);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}