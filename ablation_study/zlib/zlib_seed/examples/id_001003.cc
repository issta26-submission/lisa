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
//<ID> 1003
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char payload[] = "zlib API sequence payload exercising deflateSetHeader, gztell, gzdirect, gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    z_stream dstr;
    memset(&dstr, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstr, 6, zlibVersion(), (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (size_t)sizeof(gz_header));
    header.text = 1;
    header.time = (uLong)123456789;
    header.xflags = 2;
    header.os = 255;
    header.name = (Bytef *)"testname";
    header.comment = (Bytef *)"testcomment";
    header.hcrc = 0;
    int rc_set_header = deflateSetHeader(&dstr, &header);

    // step 2: Configure
    uLong bound = deflateBound(&dstr, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    dstr.next_in = (Bytef *)src_buf;
    dstr.avail_in = (uInt)payload_len;
    dstr.next_out = comp_buf;
    dstr.avail_out = (uInt)bound;

    // step 3: Operate
    int rc_deflate = deflate(&dstr, 0);
    uLong compressed_len = (uLong)dstr.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)payload_len);
    off_t pos_after_write = gztell(gw);
    int rc_gzdirect = gzdirect(gw);
    int rc_gzclose = gzclose(gw);

    // step 4: Validate & Cleanup
    int rc_deflate_end = deflateEnd(&dstr);
    delete [] comp_buf;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)compressed_len;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_gzdirect;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}