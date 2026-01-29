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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong compBound = deflateBound(&strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compBound;
    deflate(&strm, 4);
    deflateResetKeep(&strm);

    // step 3: Operate & Validate
    unsigned char small_input[16];
    memset(small_input, 'B', sizeof(small_input));
    uLong smallLen = (uLong)sizeof(small_input);
    strm.next_in = (Bytef *)small_input;
    strm.avail_in = (uInt)smallLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compBound;
    deflate(&strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'A');
    gzputc(gz, '\n');
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&strm);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}