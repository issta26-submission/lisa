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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    uLongf compLen = (uLongf)bound;
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    unsigned char *decomp = (unsigned char *)malloc((size_t)inputLen);
    memset(decomp, 0, (size_t)inputLen);

    // step 2: Configure
    compress((Bytef *)comp, &compLen, (const Bytef *)input, inputLen);
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    infstrm.next_in = (Bytef *)comp;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = (Bytef *)decomp;
    infstrm.avail_out = (uInt)inputLen;

    // step 3: Operate & Validate
    inflate(&infstrm, 0);
    unsigned int out_len = (unsigned int)infstrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)decomp, out_len);
    gzflush(gz, 2);

    // step 4: Cleanup
    inflateEnd(&infstrm);
    gzclose(gz);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}