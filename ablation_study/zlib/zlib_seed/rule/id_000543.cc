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
//<ID> 543
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

    // step 2: Initialize & perform deflate, then cleanup deflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLong compProduced = (uLong)(bound - (uLong)defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist original input to a .gz file and read it back using gzread
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite((voidpc)input, (z_size_t)1, (z_size_t)inputLen, gw);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    unsigned char readBuf[256];
    memset(readBuf, 0, sizeof(readBuf));
    int got = gzread(gr, (voidp)readBuf, (unsigned int)inputLen);
    (void)got;
    gzclose(gr);

    // step 4: Initialize inflate, query header, set dictionary, inflate compressed data, validate & cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    gz_header gzh;
    memset(&gzh, 0, sizeof(gzh));
    inflateGetHeader(&infStrm, &gzh);
    inflateSetDictionary(&infStrm, (const Bytef *)readBuf, (uInt)4);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compProduced;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, Z_FINISH);
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);

    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}