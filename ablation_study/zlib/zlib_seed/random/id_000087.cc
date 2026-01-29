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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infbackstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infbackstrm, 0, sizeof(z_stream));
    const char sample[] = "zlib API sequence sample payload for deflate/inflateBack and checksum";
    const Bytef *src = (const Bytef *)sample;
    uLong srcLen = (uLong)(sizeof(sample) - 1);
    uLong bound = compressBound(srcLen);
    Bytef *srcBuf = (Bytef *)malloc((size_t)srcLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memcpy(srcBuf, src, (size_t)srcLen);
    memset(compBuf, 0, (size_t)bound);
    memset(window, 0, (size_t)(1 << 15));
    defstrm.next_in = srcBuf;
    defstrm.avail_in = (uInt)srcLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;

    // step 2: Setup (initialize and configure deflate & inflateBack)
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, sizeof(gz_header));
    gzhead.text = 1;
    gzhead.time = 0;
    int rc_set_header = deflateSetHeader(&defstrm, &gzhead);
    int rc_infb_init = inflateBackInit_(&infbackstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (compress a block and compute adler32_z checksum)
    int rc_deflate = deflate(&defstrm, 0);
    uLong checksum = adler32_z(1UL, srcBuf, (z_size_t)srcLen);

    // step 4: Validate & Cleanup
    (void)rc_def_init; (void)rc_set_header; (void)rc_infb_init; (void)rc_deflate; (void)checksum;
    int rc_def_end = deflateEnd(&defstrm);
    int rc_infb_end = inflateBackEnd(&infbackstrm);
    free(srcBuf);
    free(compBuf);
    free(window);
    // API sequence test completed successfully
    (void)rc_def_end; (void)rc_infb_end;
    return 66;
}