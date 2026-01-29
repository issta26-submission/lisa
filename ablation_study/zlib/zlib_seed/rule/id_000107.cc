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
//<ID> 107
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
    unsigned char input[64];
    unsigned char decomp[64];
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure (compress then prepare inflate)
    compress2(comp, &compLen, input, inputLen, 6);
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)comp;
    strm.avail_in = (uInt)compLen;
    strm.next_out = (Bytef *)decomp;
    strm.avail_out = (uInt)sizeof(decomp);

    // step 3: Operate & Validate (inflate, write compressed to gz, flush)
    inflate(&strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)comp, (unsigned int)compLen);
    gzflush(gz, 2);
    (void)gzerror(gz, (int *)0);

    // step 4: Cleanup
    inflateEnd(&strm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}