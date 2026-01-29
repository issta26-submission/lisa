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
//<ID> 602
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = (uLong)64;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLongf destCap = (uLongf)compressBound(INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)destCap);
    memset(comp, 0, (size_t)destCap);
    Bytef *decomp_out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(decomp_out, 0, (size_t)INPUT_LEN);

    // step 2: Setup & compress
    uLongf compLen = destCap;
    compress(comp, &compLen, input, INPUT_LEN);

    // step 3: Configure & operate
    memset(&strm, 0, sizeof(strm));
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    strm.next_out = decomp_out;
    strm.avail_out = (uInt)INPUT_LEN;
    uLong crc1 = crc32(0L, input, (uInt)INPUT_LEN);
    uLong crc2 = crc32(0L, comp, (uInt)compLen);
    uLong combined = crc32_combine(crc1, crc2, (off_t)INPUT_LEN);
    (void)combined;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");

    // step 4: Cleanup
    gzwrite(gz, input, (unsigned int)16);
    gzclose_w(gz);
    inflateEnd(&strm);
    free(input);
    free(comp);
    free(decomp_out);
    // API sequence test completed successfully
    return 66;
}