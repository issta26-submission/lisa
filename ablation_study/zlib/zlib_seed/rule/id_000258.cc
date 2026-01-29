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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char input[256];
    memset(input, 'X', sizeof(input));
    uLong inLen = (uLong)sizeof(input);
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&defstrm, inLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Core operations
    deflate(&defstrm, 0);
    inflateUndermine(&infstrm, 1);
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz1, compBuf, (unsigned int)defstrm.total_out);
    gzFile gz2 = gzdopen(1, "wb");
    gzwrite(gz2, compBuf, (unsigned int)defstrm.total_out);
    inflateBackEnd(&infstrm);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    gzclose(gz1);
    gzclose(gz2);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}