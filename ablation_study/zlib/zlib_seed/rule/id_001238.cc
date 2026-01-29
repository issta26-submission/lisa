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
//<ID> 1238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute checksum
    const uLong DATA_LEN = 100UL;
    Bytef *dict = (Bytef *)malloc((size_t)DATA_LEN);
    memset(dict, 'D', (size_t)DATA_LEN);
    uLong dict_crc = crc32_z(0UL, dict, (z_size_t)DATA_LEN);

    // step 2: Initialize and configure inflate stream
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 1, (int)(dict_crc & 1UL));
    inflateSetDictionary(&istrm, dict, (uInt)DATA_LEN);

    // step 3: Operate the inflate stream with a minimal input and capture output
    Bytef *inbuf = (Bytef *)malloc(1);
    inbuf[0] = 0;
    istrm.next_in = inbuf;
    istrm.avail_in = 1;
    Bytef *outbuf = (Bytef *)malloc((size_t)DATA_LEN);
    memset(outbuf, 0, (size_t)DATA_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)DATA_LEN;
    inflate(&istrm, 0);

    // step 4: Validate/cleanup
    inflateEnd(&istrm);
    free(dict);
    free(inbuf);
    free(outbuf);
    // API sequence test completed successfully
    return 66;
}