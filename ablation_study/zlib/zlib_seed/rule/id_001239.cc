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
//<ID> 1239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and compute CRC
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong orig_crc = crc32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Create a small dictionary extracted from the original data and compute its CRC
    const uInt DICT_LEN = 32U;
    Bytef *dict = (Bytef *)malloc((size_t)DICT_LEN);
    memcpy(dict, orig + (ORIG_LEN - DICT_LEN), (size_t)DICT_LEN);
    uLong dict_crc = crc32_z(0UL, dict, (z_size_t)DICT_LEN);

    // step 3: Initialize and configure inflate stream: init, prime bits, and set dictionary
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 5);
    inflateSetDictionary(&istrm, dict, (uInt)DICT_LEN);

    // step 4: Validate (use computed CRCs) and cleanup inflate and allocated memory
    (void)orig_crc;
    (void)dict_crc;
    inflateEnd(&istrm);
    free(orig);
    free(dict);
    // API sequence test completed successfully
    return 66;
}