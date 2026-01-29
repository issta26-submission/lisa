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
//<ID> 1237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare dictionary data and compute its CRC
    const uLong DICT_LEN = 64UL;
    Bytef *dictionary = (Bytef *)malloc((size_t)DICT_LEN);
    memset(dictionary, 'D', (size_t)DICT_LEN);
    uLong dict_crc = crc32_z(0UL, dictionary, (z_size_t)DICT_LEN);

    // step 2: Initialize inflate stream
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Configure inflate with dictionary and prime the bit buffer
    inflateSetDictionary(&istrm, dictionary, (uInt)DICT_LEN);
    int prime_bits = (int)(dict_crc & 7UL);
    int prime_value = (int)(dict_crc & 0xFFFFUL);
    inflatePrime(&istrm, prime_bits, prime_value);

    // step 4: Operate and cleanup: perform a no-op inflate, then end and free resources
    istrm.next_in = NULL;
    istrm.avail_in = 0;
    istrm.next_out = NULL;
    istrm.avail_out = 0;
    inflate(&istrm, 0);
    inflateEnd(&istrm);
    free(dictionary);
    // API sequence test completed successfully
    return 66;
}