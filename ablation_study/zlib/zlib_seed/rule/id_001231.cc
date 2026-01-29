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
//<ID> 1231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffer and compute CRC
    const uLong DATA_LEN = 256UL;
    Bytef *data = (Bytef *)malloc((size_t)DATA_LEN);
    memset(data, 0x5A, (size_t)DATA_LEN);
    uLong computed_crc = crc32_z(0UL, data, (z_size_t)DATA_LEN);

    // step 2: Initialize inflate stream
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Configure inflate with dictionary and prime
    uInt dictLen = (uInt)64;
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memcpy(dict, data, (size_t)dictLen);
    inflateSetDictionary(&istrm, dict, dictLen);
    inflatePrime(&istrm, 1, 1);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(dict);
    free(data);
    // API sequence test completed successfully
    return 66;
}