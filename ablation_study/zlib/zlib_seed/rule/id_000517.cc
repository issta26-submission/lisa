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
//<ID> 517
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
    const unsigned char sample[] = "Sample data for zlib compression and header inspection.";
    uLong inputLen = (uLong)sizeof(sample);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the input into compBuf
    compress2(compBuf, &compLen, (const Bytef *)sample, inputLen, 6);

    // step 3: Write compressed data to a .gz file using gzopen + gzfwrite, then initialize inflate and get header
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite(compBuf, (z_size_t)1, (z_size_t)compLen, gz);
    gzclose(gz);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&infStrm, &head);

    // step 4: Setup inflate input/output, perform inflate, and cleanup
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, Z_FINISH);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}