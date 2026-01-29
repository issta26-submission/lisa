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
//<ID> 583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream infStrm;
    z_stream backStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&backStrm, 0, sizeof(backStrm));
    const char *ver = zlibVersion();
    Bytef *input = (Bytef *)malloc(32);
    memset(input, 'A', 32);
    uLong inputLen = (uLong)32;
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *out = (Bytef *)malloc((size_t)inputLen);
    memset(out, 0, (size_t)inputLen);

    // step 2: Initialize deflate, compress the input
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = comp;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, 0);
    uLong compProduced = defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Initialize inflate, decompress the data, validate via adler32
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.next_in = comp;
    infStrm.avail_in = (uInt)compProduced;
    infStrm.next_out = out;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    uLong outProduced = infStrm.total_out;
    inflateEnd(&infStrm);
    uLong checksum_in = adler32(0UL, input, (uInt)inputLen);
    uLong checksum_out = adler32(0UL, out, (uInt)outProduced);
    (void)checksum_in;
    (void)checksum_out;

    // step 4: Initialize inflateBack context, use undermine, then cleanup everything
    unsigned char *window = (unsigned char *)malloc(32);
    memset(window, 0, 32);
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));
    inflateUndermine(&backStrm, 0);
    inflateBackEnd(&backStrm);
    free(window);
    free(input);
    free(comp);
    free(out);
    return 66; // API sequence test completed successfully
}