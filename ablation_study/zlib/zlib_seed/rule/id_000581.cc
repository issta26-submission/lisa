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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    const char * ver = zlibVersion();
    Bytef * input = (Bytef *)malloc((size_t)128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLong adler = adler32(0L, input, (uInt)inputLen);

    // step 2: Initialize & compress using deflate
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, 4);
    deflateEnd(&defStrm);

    // step 3: Initialize inflate, apply undermine, and finalize inflate backend
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.adler = adler;
    inflateUndermine(&infStrm, 1);
    inflateBackEnd(&infStrm);

    // step 4: Cleanup
    free(input);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}