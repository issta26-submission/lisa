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
//<ID> 548
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
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * compBuf2 = (Bytef *)malloc((size_t)bound);
    memset(compBuf2, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize, configure & compress (deflate)
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLongf compProduced = (uLongf)(bound - defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist compressed bytes to a .gz file and read them back
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite((voidpc)compBuf, (z_size_t)1, (z_size_t)compProduced, gw);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int readBytes = gzread(gr, compBuf2, (unsigned int)bound);
    gzclose(gr);

    // step 4: Initialize inflate, set header & dictionary, perform inflate, cleanup
    inflateInit2_(&infStrm, 15 + 16, zlibVersion(), (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&infStrm, &header);
    const Bytef * dict = (const Bytef *)input;
    uInt dictLen = (uInt)4;
    inflateSetDictionary(&infStrm, dict, dictLen);
    infStrm.next_in = compBuf2;
    infStrm.avail_in = (uInt)readBytes;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, Z_FINISH);
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);

    free(compBuf);
    free(compBuf2);
    free(outBuf);

    return 66; // API sequence test completed successfully
}