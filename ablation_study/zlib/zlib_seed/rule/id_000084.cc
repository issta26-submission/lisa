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
//<ID> 84
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

    // step 2: Configure (deflate init and buffers)
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = (Bytef *)comp;
    strm.avail_out = (uInt)cb;

    // step 3: Operate & Validate (compress and write compressed bytes into a gz file)
    deflate(&strm, 0);
    unsigned int comp_len = (unsigned int)strm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gz);
    gzwrite(gz, (voidpc)comp, comp_len);
    gzflush(gz, 2);

    // step 4: Cleanup
    deflateEnd(&strm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}