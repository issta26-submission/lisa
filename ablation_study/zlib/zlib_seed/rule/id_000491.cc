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
//<ID> 491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream backStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Initialize streams & compute bounds
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&defStrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate and validate
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong compressed_crc = crc32(0UL, compBuf, (uInt)compLen);
    (void)checksum;
    (void)compressed_crc;
    gzFile gf = gzopen("zlib_test_output.gz", "wb");
    gzwrite(gf, compBuf, (unsigned int)compLen);
    gzclose(gf);

    // step 4: Cleanup & finalize
    inflateEnd(&backStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    free(window);
    return 66; // API sequence test completed successfully
}