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
//<ID> 1001
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char payload[] = "zlib API sequence payload exercising deflateSetHeader, gztell, gzdirect and gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 0;
    head.time = 0;
    head.name = (Bytef *)"test_zlib_name";
    head.comment = (Bytef *)"test_zlib_comment";
    head.hcrc = 0;

    // step 2: Configure
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &head);

    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate and Validate
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)payload_len);
    off_t pos = gztell(gw);
    int direct = gzdirect(gw);
    int rc_gzclose = gzclose(gw);

    // step 4: Cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)pos;
    (void)direct;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}