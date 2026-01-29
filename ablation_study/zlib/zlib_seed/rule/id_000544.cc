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
//<ID> 544
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    Bytef dictBuf[16];
    memset(dictBuf, 'D', sizeof(dictBuf));
    uInt dictLen = (uInt)sizeof(dictBuf);
    gz_header header;
    memset(&header, 0, sizeof(header));

    // step 2: Create a gzip file by writing original data with gzwrite
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gw, (voidpc)input, (unsigned int)inputLen);
    gzclose(gw);

    // step 3: Initialize deflate, compress into compBuf, then end deflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLongf compProduced = (uLongf)(bound - defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 4: Initialize inflate, set dictionary, get header, read from gz file, cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateSetDictionary(&infStrm, (const Bytef *)dictBuf, dictLen);
    inflateGetHeader(&infStrm, &header);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int bytesRead = gzread(gr, (voidp)outBuf, (unsigned int)inputLen);
    (void)bytesRead;
    gzclose(gr);
    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}