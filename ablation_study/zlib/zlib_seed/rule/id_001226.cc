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
//<ID> 1226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare input buffers, compute CRCs
    const uLong BUF1_LEN = 128UL;
    const uLong BUF2_LEN = 64UL;
    Bytef *buf1 = (Bytef *)malloc((size_t)BUF1_LEN);
    Bytef *buf2 = (Bytef *)malloc((size_t)BUF2_LEN);
    memset(buf1, 'A', (size_t)BUF1_LEN);
    memset(buf2, 'B', (size_t)BUF2_LEN);
    uLong crc1 = crc32(0UL, buf1, (uInt)BUF1_LEN);
    uLong crc2 = crc32(0UL, buf2, (uInt)BUF2_LEN);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)BUF2_LEN);

    // step 2: Initialize and configure zlib streams
    const char *ver = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Operate — write data to a gzip file and exercise inflate-side APIs
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, buf1, (unsigned int)BUF1_LEN);
    gzwrite(gz, buf2, (unsigned int)BUF2_LEN);
    gzflush(gz, 0);
    gzclose(gz);
    (void)inflateUndermine(&istrm, 1);
    (void)inflateBackEnd(&istrm);

    // step 4: Cleanup and validate-like use of combined checksum variable
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(buf1);
    free(buf2);
    (void)combined_crc; // used to show meaningful flow: crc combined from parts

    // API sequence test completed successfully
    return 66;
}