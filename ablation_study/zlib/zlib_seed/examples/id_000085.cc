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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infbackstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infbackstrm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib API sequence: deflateInit_, deflateSetHeader, adler32_z, inflateBackInit_";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Setup (initialize deflate and header, compute checksum)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    header.text = 1;
    header.time = 0;
    header.xflags = 0;
    header.os = 255;
    header.hcrc = 0;
    int rc_set_header = deflateSetHeader(&defstrm, &header);
    uLong checksum = adler32_z(0L, source, (z_size_t)sourceLen);

    // step 3: Core operations (compress with deflate, initialize inflate back)
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&defstrm, 4); // 4 == Z_FINISH
    int rc_inflateback_init = inflateBackInit_(&infbackstrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Cleanup & finalize
    int rc_inflateback_end = inflateBackEnd(&infbackstrm);
    int rc_def_end = deflateEnd(&defstrm);
    free(compBuf);
    free(window);
    (void)rc_def_init; (void)rc_set_header; (void)checksum; (void)rc_deflate; (void)rc_inflateback_init; (void)rc_inflateback_end; (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}