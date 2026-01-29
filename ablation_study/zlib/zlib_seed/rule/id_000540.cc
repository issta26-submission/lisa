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
//<ID> 540
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
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize & configure deflate, then perform compression
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLong compProduced = (uLong)(bound - defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist compressed data to a .gz file and read it back using gzread
    gzFile gzout = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gzout, (voidpc)compBuf, (unsigned int)compProduced);
    gzclose(gzout);
    gzFile gzin = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef * readBuf = (Bytef *)malloc((size_t)compProduced);
    memset(readBuf, 0, (size_t)compProduced);
    int readBytes = gzread(gzin, (voidp)readBuf, (unsigned int)compProduced);
    gzclose(gzin);

    // step 4: Initialize inflate, attempt to get header, set dictionary, inflate & cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = readBuf;
    infStrm.avail_in = (uInt)readBytes;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&infStrm, &header);
    Bytef dict[16];
    memset(dict, 'D', sizeof(dict));
    inflateSetDictionary(&infStrm, dict, (uInt)sizeof(dict));
    inflate(&infStrm, Z_FINISH);
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);

    // Cleanup
    free(compBuf);
    free(readBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}