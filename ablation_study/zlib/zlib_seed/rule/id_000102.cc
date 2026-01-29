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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream instrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    unsigned char input[64];
    unsigned char decomp[64];
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = (Bytef *)comp;
    defstrm.avail_out = (uInt)bound;
    deflate(&defstrm, 0);
    uInt comp_len = (uInt)defstrm.total_out;
    inflateInit_(&instrm, zlibVersion(), (int)sizeof(z_stream));
    instrm.next_in = (Bytef *)comp;
    instrm.avail_in = comp_len;
    instrm.next_out = (Bytef *)decomp;
    instrm.avail_out = (uInt)sizeof(decomp);

    // step 3: Operate & Validate
    inflate(&instrm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp, comp_len);
    gzflush(gz, 2);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&instrm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}