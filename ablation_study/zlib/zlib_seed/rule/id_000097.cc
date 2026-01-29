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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream defcopy;
    z_stream backstrm;
    z_stream backcopy;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&defcopy, 0, sizeof(defcopy));
    memset(&backstrm, 0, sizeof(backstrm));
    memset(&backcopy, 0, sizeof(backcopy));
    unsigned char input[256];
    unsigned char decomp[256];
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure (deflate to create compressed data)
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = (Bytef *)comp;
    defstrm.avail_out = (uInt)bound;
    deflate(&defstrm, 0);
    unsigned int comp_len = (unsigned int)defstrm.total_out;

    // step 3: Operate & Validate (copy deflate state, init inflateBack, copy inflate state, validate/undermine)
    deflateCopy(&defcopy, &defstrm);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&backcopy, &backstrm);
    inflateValidate(&backcopy, 1);
    inflateUndermine(&backcopy, 1);

    // step 4: Cleanup
    inflateBackEnd(&backstrm);
    deflateEnd(&defstrm);
    deflateEnd(&defcopy);
    inflateEnd(&backcopy);
    free(comp);

    // API sequence test completed successfully
    return 66;
}