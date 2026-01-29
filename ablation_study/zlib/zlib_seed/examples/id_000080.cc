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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib API sequencing: deflate + inflateBackInit_ + checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // allocate buffers
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    Bytef *nameBuf = (Bytef *)malloc(8);

    // prepare buffers
    memset(compBuf, 0, (size_t)bound);
    memset(window, 0, (size_t)(1 << 15));
    memset(nameBuf, 0, 8);
    memcpy(nameBuf, "tname", 6);

    // set up deflate stream input/output pointers
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;

    // step 2: Setup (initialize deflate and set header)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    gz_header h;
    memset(&h, 0, sizeof(gz_header));
    h.text = 1;
    h.name = nameBuf;
    h.name_max = 7;
    int rc_set_hdr = deflateSetHeader(&defstrm, &h);

    // step 3: Operate & Validate (perform deflate, init inflate-back, compute adler32)
    int rc_deflate = deflate(&defstrm, 4);
    int rc_inf_init = inflateBackInit_(&infstrm, 15, window, version, (int)sizeof(z_stream));
    uLong checksum = adler32_z(0L, source, (z_size_t)sourceLen);

    // step 4: Cleanup
    int rc_inf_end = inflateBackEnd(&infstrm);
    int rc_def_end = deflateEnd(&defstrm);
    free(compBuf);
    free(window);
    free(nameBuf);
    (void)rc_def_init; (void)rc_set_hdr; (void)rc_deflate; (void)rc_inf_init; (void)checksum; (void)rc_inf_end; (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}