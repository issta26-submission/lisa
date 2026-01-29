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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    z_stream istrm_copy;
    z_stream istrm_back;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&dstrm_copy, 0, sizeof(dstrm_copy));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(&istrm_back, 0, sizeof(istrm_back));
    unsigned char input[128];
    unsigned char decompBuf[256];
    memset(input, 'X', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);

    // step 2: Configure (deflate to produce compressed data, then copy deflate state)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = (Bytef *)comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    unsigned int comp_len = (unsigned int)dstrm.total_out;
    deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Configure inflate, copy & validate, use inflateBackInit_ and inflateBackEnd
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = (Bytef *)decompBuf;
    istrm.avail_out = (uInt)sizeof(decompBuf);
    inflateCopy(&istrm_copy, &istrm);
    inflateValidate(&istrm_copy, 1);
    inflateUndermine(&istrm_copy, 0);
    inflate(&istrm_copy, 0);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm_back, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateBackEnd(&istrm_back);

    // step 4: Cleanup
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm);
    deflateEnd(&dstrm_copy);
    deflateEnd(&dstrm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}