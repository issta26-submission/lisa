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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream def_copy;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&def_copy, 0, sizeof(def_copy));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize deflater and copy it)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateCopy(&def_copy, &def_strm);

    // step 3: Core operations (compute checksum, run deflate on different streams)
    uLong checksum = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    (void)checksum;
    Bytef *outBuf = (Bytef *)malloc((size_t)(compBound * 2));
    Bytef *outBuf2 = (Bytef *)malloc((size_t)(compBound * 2));
    memset(outBuf, 0, (size_t)(compBound * 2));
    memset(outBuf2, 0, (size_t)(compBound * 2));
    def_strm.next_in = compBuf;
    def_strm.avail_in = (uInt)compLen;
    def_strm.next_out = outBuf;
    def_strm.avail_out = (uInt)(compBound * 2);
    deflate(&def_strm, 0);
    def_copy.next_in = (Bytef *)input;
    def_copy.avail_in = (uInt)inputLen;
    def_copy.next_out = outBuf2;
    def_copy.avail_out = (uInt)(compBound * 2);
    deflate(&def_copy, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)outBuf, (unsigned int)def_strm.total_out);
    gzwrite(gz, (voidpc)outBuf2, (unsigned int)def_copy.total_out);
    deflateEnd(&def_strm);
    deflateEnd(&def_copy);
    gzclose(gz);
    free(compBuf);
    free(outBuf);
    free(outBuf2);

    // API sequence test completed successfully
    return 66;
}