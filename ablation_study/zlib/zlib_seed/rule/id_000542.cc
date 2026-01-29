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
//<ID> 542
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
    memset(input, 'P', sizeof(input));
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
    uLongf compProduced = (uLongf)(bound - defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist original input to a .gz file and read it back with gzread
    gzFile gzw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gzw, input, (unsigned int)inputLen);
    gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef * gzReadBuf = (Bytef *)malloc((size_t)inputLen);
    memset(gzReadBuf, 0, (size_t)inputLen);
    int gzBytes = gzread(gzr, (voidp)gzReadBuf, (unsigned int)inputLen);
    (void)gzBytes;
    gzclose(gzr);

    // step 4: Initialize inflate, configure header & dictionary, perform inflate, validate & cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&infStrm, &head);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compProduced;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateSetDictionary(&infStrm, (const Bytef *)input, (uInt)32);
    inflate(&infStrm, Z_FINISH);
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);

    free(compBuf);
    free(outBuf);
    free(gzReadBuf);

    return 66; // API sequence test completed successfully
}