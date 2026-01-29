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
//<ID> 1230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffer and compute CRC
    const uLong LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)LEN);
    memset(orig, 'Z', (size_t)LEN);
    uLong crc = crc32_z(0UL, orig, (z_size_t)LEN);

    // step 2: Initialize inflate stream
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = orig;
    istrm.avail_in = (uInt)LEN;
    istrm.adler = crc;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Configure stream (prime bit buffer) and set dictionary for inflation
    inflatePrime(&istrm, 3, 5);
    inflateSetDictionary(&istrm, orig, (uInt)LEN);

    // step 4: Cleanup and finish
    inflateEnd(&istrm);
    free(orig);
    // API sequence test completed successfully
    return 66;
}