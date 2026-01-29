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
//<ID> 513
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
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compBufLen = (uLongf)bound;

    // step 2: Write original data to a gzip file
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite((voidpc)input, (z_size_t)1, (z_size_t)inputLen, gz);
    gzclose(gz);

    // step 3: Compress data and attempt to retrieve inflate header
    compress2(compBuf, &compBufLen, (const Bytef *)input, inputLen, 6);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compBufLen;
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&infStrm, &header);

    // step 4: Cleanup
    inflateEnd(&infStrm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}