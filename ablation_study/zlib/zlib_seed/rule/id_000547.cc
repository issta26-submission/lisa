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
//<ID> 547
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
    unsigned char input[256];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: Initialize deflate, compress input, and cleanup deflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLongf compProduced = (uLongf)(bound - defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist compressed bytes into a .gz file and read them back with gzread
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite((voidpc)compBuf, (z_size_t)1, (z_size_t)compProduced, gz);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    unsigned char readBuf[256];
    memset(readBuf, 0, sizeof(readBuf));
    gzread(gz, (voidp)readBuf, (unsigned int)sizeof(readBuf));
    gzclose(gz);

    // step 4: Initialize inflate, set dictionary, get header, validate & cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef dict[32];
    memset(dict, 'D', sizeof(dict));
    inflateSetDictionary(&infStrm, dict, (uInt)sizeof(dict));
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&infStrm, &header);
    inflateEnd(&infStrm);

    free(compBuf);

    return 66; // API sequence test completed successfully
}