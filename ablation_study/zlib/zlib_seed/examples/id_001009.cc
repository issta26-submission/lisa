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
//<ID> 1009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising deflateSetHeader, gztell, gzdirect, gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Setup deflate stream and header
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.time = (uLong)0x5A5A5A5A;
    static Bytef name_buf[] = { 't','e','s','t','_','h','d','r',0 };
    head.name = name_buf;
    head.name_max = (uInt)(sizeof(name_buf));
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &head);

    // step 3: Operate (deflate compressed input to output buffer) and use gzFile APIs
    uLong out_bound = deflateBound(&dstrm, (uLong)comp_len_f);
    Bytef *out_buf = new Bytef[(out_bound ? out_bound : 1)];
    dstrm.next_in = comp_buf;
    dstrm.avail_in = (uInt)comp_len_f;
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)out_bound;
    int rc_deflate = deflate(&dstrm, 0);

    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    off_t pos_after_write = gztell(gf);
    int is_direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] comp_buf;
    delete [] out_buf;

    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)is_direct;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}