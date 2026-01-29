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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defStrm;
    z_stream defCopyStrm;
    z_stream infStrm;
    z_stream infCopyStrm;
    z_stream backStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&defCopyStrm, 0, sizeof(defCopyStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&infCopyStrm, 0, sizeof(infCopyStrm));
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    unsigned char decomp[128];
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong cb = compressBound(inputLen);
    unsigned char *compBuf = (unsigned char *)malloc((size_t)cb);
    memset(compBuf, 0, (size_t)cb);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));

    // step 2: Configure deflate & produce compressed data, then copy deflate state
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = (Bytef *)compBuf;
    defStrm.avail_out = (uInt)cb;
    deflate(&defStrm, 0);
    deflateCopy(&defCopyStrm, &defStrm);

    // step 3: Configure inflate, operate, copy and validate, use inflateBack init/end and undermine
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = (Bytef *)compBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    infStrm.next_out = (Bytef *)decomp;
    infStrm.avail_out = (uInt)sizeof(decomp);
    inflate(&infStrm, 0);
    inflateCopy(&infCopyStrm, &infStrm);
    inflateValidate(&infCopyStrm, 1);
    inflateUndermine(&infCopyStrm, 1);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateBackEnd(&backStrm);

    // step 4: Cleanup
    inflateEnd(&infStrm);
    inflateEnd(&infCopyStrm);
    deflateEnd(&defStrm);
    deflateEnd(&defCopyStrm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}