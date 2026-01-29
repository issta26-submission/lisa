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
//<ID> 545
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
    Bytef * readBuf = (Bytef *)malloc((size_t)(inputLen + 64));
    memset(readBuf, 0, (size_t)(inputLen + 64));

    // step 2: Initialize & configure deflate, then perform compression
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    unsigned int compProduced = (unsigned int)(bound - defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist compressed data to a .gz file and read it back
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compProduced);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int bytesRead = gzread(gz, (voidp)readBuf, (unsigned int)(compProduced + 16));
    (void)bytesRead;
    gzclose(gz);

    // step 4: Initialize inflate, use inflateGetHeader and inflateSetDictionary, then inflate & cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compProduced;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&infStrm, &head);
    Bytef dict[16];
    memset(dict, 'D', sizeof(dict));
    inflateSetDictionary(&infStrm, dict, (uInt)sizeof(dict));
    inflate(&infStrm, Z_FINISH);
    inflateEnd(&infStrm);

    free(compBuf);
    free(outBuf);
    free(readBuf);

    // API sequence test completed successfully
    return 66;
}