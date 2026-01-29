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
//<ID> 891
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence test payload";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLongf comp_bound = (uLongf)compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    uLongf comp_len = comp_bound;
    z_stream dstream;
    z_stream istream;
    gz_header head;
    memset(&dstream, 0, (size_t)sizeof(z_stream));
    memset(&istream, 0, (size_t)sizeof(z_stream));
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.name = (Bytef *)"zlib_test_name";

    // step 2: Configure compression and header
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);
    int rc_deflate_init = deflateInit_(&dstream, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstream, &head);
    int rc_inflate_init = inflateInit_(&istream, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate using gz* IO and validate inflate mark
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gw, "payload_len=%lu\n", (unsigned long)payload_len);
    unsigned int write_len = (unsigned int)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gw, (voidpc)comp_buf, write_len);
    int rc_gzflush = gzflush(gw, 0);
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned int read_buf_size = (unsigned int)(((uLong)payload_len + 16UL) <= (uLong)~0u ? ((uLong)payload_len + 16UL) : (uLong)~0u);
    Bytef *read_buf = new Bytef[(read_buf_size ? read_buf_size : 1)];
    int rc_gzread = gzread(gr, (voidp)read_buf, read_buf_size);
    long inflate_mark = inflateMark(&istream);
    int rc_gzclose_r = gzclose(gr);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istream);
    int rc_deflate_end = deflateEnd(&dstream);
    delete [] comp_buf;
    delete [] read_buf;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_inflate_init;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)inflate_mark;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}