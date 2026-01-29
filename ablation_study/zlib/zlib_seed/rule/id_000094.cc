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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream defstrm_copy;
    z_stream instrm;
    z_stream instrm_copy;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&defstrm_copy, 0, sizeof(defstrm_copy));
    memset(&instrm, 0, sizeof(instrm));
    memset(&instrm_copy, 0, sizeof(instrm_copy));
    unsigned char input[128];
    unsigned char decomp[128];
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));

    // step 2: Configure
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = (Bytef *)comp;
    defstrm.avail_out = (uInt)compBound;

    inflateBackInit_(&instrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateReset(&instrm);

    // step 3: Operate & Validate
    deflate(&defstrm, 0);
    unsigned int comp_len = (unsigned int)defstrm.total_out;
    deflateCopy(&defstrm_copy, &defstrm);
    inflateCopy(&instrm_copy, &instrm);
    inflateValidate(&instrm_copy, 1);
    inflateUndermine(&instrm_copy, 0);

    // step 4: Cleanup
    inflateBackEnd(&instrm);
    inflateBackEnd(&instrm_copy);
    deflateEnd(&defstrm_copy);
    deflateEnd(&defstrm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}