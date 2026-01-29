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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream dcopy;
    z_stream istrm;
    z_stream icopy;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&dcopy, 0, sizeof(dcopy));
    memset(&istrm, 0, sizeof(istrm));
    memset(&icopy, 0, sizeof(icopy));
    unsigned char input[128];
    unsigned char decomp[128];
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *compBuf = (unsigned char *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: Configure (create compressed data and copy deflate state)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = (Bytef *)compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 0);
    deflateCopy(&dcopy, &dstrm);

    // step 3: Operate & Validate (inflate, copy inflate state, validate and undermine, call inflateBackEnd)
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = (Bytef *)decomp;
    istrm.avail_out = (uInt)sizeof(decomp);
    inflate(&istrm, 0);
    inflateCopy(&icopy, &istrm);
    inflateValidate(&icopy, 1);
    inflateUndermine(&icopy, 1);
    inflateBackEnd(&icopy);

    // step 4: Cleanup
    inflateEnd(&istrm);
    inflateEnd(&icopy);
    deflateEnd(&dstrm);
    deflateEnd(&dcopy);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}