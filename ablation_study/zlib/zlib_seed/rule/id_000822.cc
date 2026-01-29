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
//<ID> 822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512UL;
    const uLong EXTRA_LEN = 128UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    Bytef *extra = (Bytef *)malloc((size_t)EXTRA_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    memset(extra, 'E', (size_t)EXTRA_LEN);

    // step 2: Create a gzip file and write two blocks, then flush to ensure data is committed
    gzFile gz = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)INPUT_LEN);
    gzwrite(gz, (voidpc)extra, (unsigned int)EXTRA_LEN);
    gzflush(gz, 2); /* Z_SYNC_FLUSH */
    gzclose(gz);

    // step 3: Initialize an inflate stream, prime bit buffer, and compute CRC combine for validation
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = input;
    istrm.avail_in = (uInt)INPUT_LEN;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 5);

    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    uLong crc_extra = crc32(0UL, extra, (uInt)EXTRA_LEN);
    uLong crc_combined = crc32_combine(crc_input, crc_extra, (off_t)EXTRA_LEN);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(input);
    free(extra);
    (void)ver;
    (void)crc_input;
    (void)crc_extra;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}