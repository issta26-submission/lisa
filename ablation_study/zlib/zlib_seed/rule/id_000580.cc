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
//<ID> 580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    const char *ver = zlibVersion();
    Bytef *input = (Bytef *)malloc((size_t)128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Initialize streams
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = comp;
    defStrm.avail_out = (uInt)bound;
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));

    // step 3: Operate & validate
    deflate(&defStrm, 4);
    uLong chk = adler32(0L, input, (uInt)inputLen);
    defStrm.adler = chk;
    infStrm.next_in = comp;
    infStrm.avail_in = (uInt)defStrm.total_out;
    infStrm.adler = chk;
    inflateUndermine(&infStrm, 1);
    inflateBackEnd(&infStrm);

    // step 4: Cleanup
    inflateEnd(&infStrm);
    deflateEnd(&defStrm);
    free(input);
    free(comp);
    // API sequence test completed successfully
    return 66;
}