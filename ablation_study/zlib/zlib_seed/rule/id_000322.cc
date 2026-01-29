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
//<ID> 322
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&defstrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    Bytef * outBuf = (Bytef *)malloc(256);
    memset(outBuf, 0, 256);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)256;

    // step 3: Operate & Validate
    deflate(&defstrm, 4);
    unsigned int compLen = (unsigned int)defstrm.total_out;
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    inflatePrime(&infstrm, 1, 0);
    inflate(&infstrm, 0);
    inflateValidate(&infstrm, 15);
    long mark = inflateMark(&infstrm);
    (void)mark;

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}