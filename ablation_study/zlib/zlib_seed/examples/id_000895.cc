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
//<ID> 895
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib sequence payload for compress, header, gz IO and inflateMark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, (size_t)sizeof(gz_header));
    gzhead.name = (Bytef *)"testname";
    gzhead.name_max = (uInt)8;
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &gzhead);

    // step 2: Configure and write compressed data with gzprintf
    gzFile gf_write = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf_write, "zlib-metadata\n");
    unsigned int write_len = (unsigned int)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf_write, comp_buf, write_len);
    int rc_gzflush = gzflush(gf_write, 0);
    int rc_gzclose_w = gzclose(gf_write);

    // step 3: Operate - read back and inspect inflate mark
    gzFile gf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned int read_buf_len = (unsigned int)((comp_len + 64UL) <= (uLong)~0u ? (comp_len + 64UL) : (uLong)~0u);
    Bytef *read_buf = new Bytef[(read_buf_len ? read_buf_len : 1)];
    int rc_gzread = gzread(gf_read, (voidp)read_buf, read_buf_len);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(rc_gzread >= 0 ? (unsigned int)rc_gzread : 0u);
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose_r = gzclose(gf_read);
    delete [] comp_buf;
    delete [] read_buf;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}