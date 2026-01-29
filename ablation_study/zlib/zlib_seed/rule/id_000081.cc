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
//<ID> 81
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
    memset(input, 'A', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    unsigned int comp_len = 0;

    // step 2: Configure (initialize deflate and set buffers)
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = (Bytef *)comp;
    strm.avail_out = (uInt)cb;
    deflate(&strm, 0);
    comp_len = (unsigned int)strm.total_out;

    // step 3: Operate & Validate (write original data to a gz file, flush and clear errors)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)sourceLen);
    gzflush(gz, 0);
    gzclearerr(gz);

    // step 4: Cleanup
    deflateEnd(&strm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}