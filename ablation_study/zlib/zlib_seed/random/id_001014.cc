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
//<ID> 1014
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising deflateEnd, gzwrite, inflate, inflateBackEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, payload_len, 6);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    z_stream bstrm;
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    unsigned char *back_window = new unsigned char[1];
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, back_window, zlibVersion(), (int)sizeof(z_stream));

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure (prepare streams and buffers)
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;

    dstrm.next_in = NULL;
    dstrm.avail_in = 0;
    dstrm.next_out = NULL;
    dstrm.avail_out = 0;

    // step 3: Operate (inflate compressed data and write original payload to gz file)
    int rc_inflate = inflate(&istrm, 0);

    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzclose = gzclose(gf);

    int rc_inflate_back_end = inflateBackEnd(&bstrm);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);

    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] back_window;

    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_back_init;
    (void)rc_deflate_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_back_end;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}