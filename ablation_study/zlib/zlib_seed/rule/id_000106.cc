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
//<ID> 106
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
    unsigned char comp[256];
    unsigned char decomp[64];
    memset(input, 'A', sizeof(input));
    memset(comp, 0, sizeof(comp));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLongf compLen = (uLongf)sizeof(comp);
    const char * ver = zlibVersion();
    compress2((Bytef *)comp, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Configure
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)comp;
    strm.avail_in = (uInt)compLen;
    strm.next_out = (Bytef *)decomp;
    strm.avail_out = (uInt)sizeof(decomp);

    // step 3: Operate & Validate
    inflate(&strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)comp, (unsigned int)compLen);
    gzflush(gz, 2);

    // step 4: Cleanup
    inflateEnd(&strm);
    gzclose(gz);

    // API sequence test completed successfully
    return 66;
}