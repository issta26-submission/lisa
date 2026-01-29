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
//<ID> 896
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare and write a small gzip file using gzprintf
    const char write_payload[] = "zlib test payload for gzprintf/gzread and stream markers";
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gwprintf = gzprintf(gw, write_payload);
    int rc_gwflush = gzflush(gw, 0);
    int rc_gwclose_w = gzclose_w ? gzclose_w(gw) : gzclose(gw);

    // step 2: Reopen for reading and read the content with gzread
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char read_buf[256];
    memset(read_buf, 0, (size_t)sizeof(read_buf));
    int rc_gzread = gzread(gr, (voidp)read_buf, (unsigned int)(sizeof(read_buf) - 1));
    int rc_gzclose_r = gzclose_r ? gzclose_r(gr) : gzclose(gr);

    // step 3: Initialize an inflate stream, set its input from the gzread buffer and get an inflate mark
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)read_buf;
    istrm.avail_in = (uInt)(rc_gzread >= 0 ? rc_gzread : 0);
    long mark_before = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Initialize a deflate stream, set a gzip header, then clean up
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, (size_t)sizeof(gz_header));
    char name_buf[] = "test_name";
    char comment_buf[] = "test_comment";
    gzhead.name = (Bytef *)name_buf;
    gzhead.name_max = (uInt)(sizeof(name_buf) - 1);
    gzhead.comment = (Bytef *)comment_buf;
    gzhead.comm_max = (uInt)(sizeof(comment_buf) - 1);
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &gzhead);
    int rc_deflate_end = deflateEnd(&dstrm);

    (void)rc_gwprintf;
    (void)rc_gwflush;
    (void)rc_gwclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)mark_before;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}