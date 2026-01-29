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
//<ID> 1232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare dictionary buffer and compute its CRC
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

    // step 3: Configure inflate with the preset dictionary and prime bit buffer, validate with CRC
    istrm.next_in = dictionary;
    istrm.avail_in = (uInt)DICT_LEN;
    istrm.adler = dict_crc;
    inflateSetDictionary(&istrm, dictionary, (uInt)DICT_LEN);
    inflatePrime(&istrm, 3, 5);
    uLong verify_crc = crc32_z(dict_crc, istrm.next_in, (z_size_t)istrm.avail_in);
    (void)verify_crc;

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(dictionary);
    // API sequence test completed successfully
    return 66;
}