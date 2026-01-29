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
//<ID> 515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    gz_header head;
    memset(&head, 0, sizeof(head));

    // step 2: Initialize & Configure
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateGetHeader(&infStrm, &head);

    // step 3: Operate & Validate
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite(compBuf, (z_size_t)1, (z_size_t)compLen, gz);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&infStrm);
    free(compBuf);

    return 66; // API sequence test completed successfully
}