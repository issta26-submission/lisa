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
//<ID> 103
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
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure (compress data and write to a gz file)
    compress2((Bytef *)comp, &compLen, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)comp, (unsigned int)compLen);
    gzflush(gz, 2);
    gzclose(gz);

    // step 3: Operate & Validate (inflate the compressed buffer)
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    unsigned char decomp[64];
    memset(decomp, 0, sizeof(decomp));
    strm.next_in = (Bytef *)comp;
    strm.avail_in = (uInt)compLen;
    strm.next_out = (Bytef *)decomp;
    strm.avail_out = (uInt)sizeof(decomp);
    inflate(&strm, 0);

    // step 4: Cleanup
    inflateEnd(&strm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}