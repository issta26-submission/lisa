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
//<ID> 868
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong BUFLEN = 64UL;
    Bytef *buf = (Bytef *)malloc((size_t)BUFLEN);
    memset(buf, 'B', (size_t)BUFLEN);
    uLong initial_crc = crc32(0UL, buf, (uInt)BUFLEN);

    // step 2: Initialize and configure streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.adler = initial_crc;
    deflateResetKeep(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.adler = initial_crc;

    // step 3: Operate & validate (write via gz, compute checksum again, propagate)
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, buf, (unsigned int)BUFLEN);
    uLong crc_again = crc32(0UL, buf, (uInt)BUFLEN);
    istrm.total_in = (uLong)crc_again;
    dstrm.total_out = (uLong)crc_again;

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    gzclose(gz);
    free(buf);
    (void)ver;
    (void)initial_crc;
    (void)crc_again;
    // API sequence test completed successfully
    return 66;
}