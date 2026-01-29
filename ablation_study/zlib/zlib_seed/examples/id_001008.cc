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
//<ID> 1008
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising deflateSetHeader, gztell, gzdirect and gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    const char name_buf[] = "test_name";
    const char comment_buf[] = "test_comment";
    head.name = (Bytef *)name_buf;
    head.name_max = (uInt)(sizeof(name_buf));
    head.comment = (Bytef *)comment_buf;
    head.comm_max = (uInt)(sizeof(comment_buf));
    head.time = (uLong)0x5A5A5A5A;
    head.text = 0;
    head.hcrc = 0;

    // step 2: Configure (attach header to deflate stream and prepare output)
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    Bytef *out_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_in = comp_buf;
    dstrm.avail_in = (uInt)comp_len_f;
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate (deflate compressed data, write gz file, query file state)
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)payload_len);
    off_t pos_after_write = gztell(gw);
    int direct_flag = gzdirect(gw);
    int rc_gzclose = gzclose(gw);

    // step 4: Cleanup and Validate
    delete [] comp_buf;
    delete [] out_buf;

    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)direct_flag;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}