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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and write a gz file
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)inputLen);
    gzclose(gz);

    // step 2: Reopen for reading, read one byte and compute CRCs
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    int c = gzgetc_(gz);
    unsigned char ch = (unsigned char)c;
    uLong crc_full = crc32_z(0UL, (const Bytef *)input, (z_size_t)inputLen);
    uLong crc_updated = crc32_z(crc_full, (const Bytef *)&ch, (z_size_t)1);

    // step 3: Initialize inflate streams, apply reset-keep, init/back-end the back stream
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateResetKeep(&infStrm);
    unsigned char window[32768];
    memset(window, 0, 32768);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateBackEnd(&backStrm);

    // step 4: Validate result flow and cleanup
    (void)crc_updated;
    inflateEnd(&infStrm);
    gzclose(gz);
    return 66;
    // API sequence test completed successfully
}