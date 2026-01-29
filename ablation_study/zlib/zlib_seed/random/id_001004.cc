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
//<ID> 1004
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char payload[] = "zlib API sequence payload exercising deflateSetHeader, gzclose, gztell, gzdirect";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15 + 16, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    gz_header header;
    memset(&header, 0, (size_t)sizeof(gz_header));
    header.text = 0;
    header.time = 0;
    header.xflags = 0;
    header.os = 255;
    header.extra = NULL;
    header.extra_len = 0;
    header.extra_max = 0;
    header.name = (Bytef *)"zlib_generated_name";
    header.name_max = 0;
    header.comment = (Bytef *)"zlib_generated_comment";
    header.comm_max = 0;
    header.hcrc = 0;
    header.done = 0;

    int rc_set_header = deflateSetHeader(&dstrm, &header);

    // step 2: Configure
    Bytef *def_out = new Bytef[(bound ? bound : 1)];
    memset(def_out, 0, (size_t)(bound ? bound : 1));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = def_out;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 0);

    // step 3: Operate (gzFile operations)
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)payload_len);
    off_t pos_after_write = gztell(gw);
    int direct_mode = gzdirect(gw);
    int rc_gzclose = gzclose(gw);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] comp_buf;
    delete [] def_out;

    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)direct_mode;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}