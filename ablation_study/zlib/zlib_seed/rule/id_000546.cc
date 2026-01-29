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
//<ID> 546
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
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize & perform deflate, then cleanup deflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLong compProduced = (uLong)(bound - (uLong)defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist compressed data to .gz, then read it back using gzread
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gw, compBuf, (unsigned int)compProduced);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef *compBufRead = (Bytef *)malloc((size_t)bound);
    memset(compBufRead, 0, (size_t)bound);
    int readBytes = gzread(gr, (voidp)compBufRead, (unsigned int)bound);
    gzclose(gr);

    // step 4: Initialize inflate, use inflateSetDictionary & inflateGetHeader, perform inflate & cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBufRead;
    infStrm.avail_in = (uInt)readBytes;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateSetDictionary(&infStrm, (const Bytef *)NULL, (uInt)0);
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&infStrm, &header);
    inflate(&infStrm, Z_FINISH);
    inflateEnd(&infStrm);

    free(compBuf);
    free(compBufRead);
    free(outBuf);

    return 66; // API sequence test completed successfully
}