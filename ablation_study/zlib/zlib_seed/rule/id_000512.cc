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
//<ID> 512
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
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    gz_header header;
    memset(&header, 0, sizeof(header));

    // step 2: Initialize & Configure (compress input and prepare gzip file)
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite((voidpc)compBuf, (z_size_t)1, (z_size_t)compLen, gz);
    gzclose(gz);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;

    // step 3: Operate & Validate (attempt to get header from the inflate stream)
    inflateGetHeader(&infStrm, &header);

    // step 4: Cleanup
    inflateEnd(&infStrm);
    free(compBuf);

    return 66; // API sequence test completed successfully
}