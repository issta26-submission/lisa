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
//<ID> 866
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong BUF_LEN = 128UL;
    Bytef *inbuf = (Bytef *)malloc((size_t)BUF_LEN);
    memset(inbuf, 'A', (size_t)BUF_LEN);
    uLong initial_crc = crc32(0UL, inbuf, (uInt)BUF_LEN);
    Bytef *outbuf = (Bytef *)malloc((size_t)BUF_LEN);
    memset(outbuf, 0, (size_t)BUF_LEN);

    // step 2: Initialize & configure streams
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = inbuf;
    istrm.avail_in = (uInt)BUF_LEN;
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)BUF_LEN;

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);

    // step 3: Operate & validate
    inflate(&istrm, 4);
    uLong out_crc = crc32(0UL, outbuf, (uInt)(istrm.total_out ? (uInt)istrm.total_out : (uInt)BUF_LEN));
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, outbuf, (unsigned int)BUF_LEN);
    gzflush(gz, 2);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(inbuf);
    free(outbuf);
    (void)ver;
    (void)initial_crc;
    (void)out_crc;
    // API sequence test completed successfully
    return 66;
}