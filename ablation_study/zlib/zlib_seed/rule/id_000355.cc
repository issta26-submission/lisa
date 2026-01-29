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
//<ID> 355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Configure inflateBack
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    inflateUndermine(&istrm, 1);
    inflateBack(&istrm, (in_func)0, NULL, (out_func)0, NULL);
    unsigned long used = inflateCodesUsed(&istrm);
    (void)used;
    inflateBackEnd(&istrm);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}