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
//<ID> 571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare version string, streams and initialize inflate-related state
    const char src[] = "zlib API payload: inflateInit_ -> inflateBackInit_ -> gz write/seek -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream inf_stream;
    z_stream back_stream;
    memset(&inf_stream, 0, (int)sizeof(z_stream));
    memset(&back_stream, 0, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&inf_stream, version, (int)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_inf_back_init = inflateBackInit_(&back_stream, 15, window, version, (int)sizeof(z_stream));

    // step 2: Configure input/output buffers and perform a nominal inflate operation
    uInt outBufSize = (uInt)4096;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outBuf, 0, (size_t)outBufSize);
    inf_stream.next_in = (Bytef *)source;
    inf_stream.avail_in = (uInt)sourceLen;
    inf_stream.next_out = outBuf;
    inf_stream.avail_out = outBufSize;
    int rc_inflate = inflate(&inf_stream, 0); /* Z_NO_FLUSH == 0 */

    // step 3: Persist the inflated output to a gzFile and query position via gztell
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)inf_stream.total_out);
    int rc_gzflush = gzflush(gf, 2); /* Z_SYNC_FLUSH == 2 */
    off_t pos = gztell(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup inflate/back state and free buffers
    int rc_inf_end = inflateEnd(&inf_stream);
    int rc_inf_back_end = inflateBackEnd(&back_stream);
    free(window);
    free(outBuf);
    (void)version;
    (void)rc_inf_init;
    (void)rc_inf_back_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_inf_back_end;
    // API sequence test completed successfully
    return 66;
}