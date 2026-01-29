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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and write compressed file via gz layer
    const char payload[] = "zlib API sequence payload for deflateBound, deflateInit_, inflateBackInit_, and gztell usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)payload, (unsigned int)payload_len);
    off_t pos_after_write = gztell(gw);
    int rc_gzclose_w = gzclose(gw);

    // step 2: Initialize deflate stream and compute worst-case bound
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 3: Perform a deflate operation and finalize deflate
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Initialize inflate-back stream with a small window and finalize, then cleanup
    z_stream bstrm;
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    unsigned char window_buf[1];
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window_buf, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&bstrm);
    delete [] comp_buf;

    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_gzclose_w;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}