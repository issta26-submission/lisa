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
//<ID> 893
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare gzip file and write payload using gzprintf
    const char payload[] = "hello zlib API sequence";
    const unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gw, "%s", payload);
    int rc_gzclose_w = gzclose(gw);

    // step 2: Open gzip file for reading and read into a buffer using gzread
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    const unsigned int buf_size = 256U;
    Bytef *read_buf = new Bytef[buf_size ? buf_size : 1];
    int rc_gzread = gzread(gr, (voidp)read_buf, buf_size);
    unsigned int read_bytes = (unsigned int)(rc_gzread >= 0 ? rc_gzread : 0U);
    read_buf[(read_bytes < buf_size) ? read_bytes : (buf_size - 1)] = 0;

    // step 3: Initialize deflate stream, set a gzip header, initialize inflate stream and mark
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    gz_header head;
    unsigned char head_name[] = "testname";
    unsigned char head_comment[] = "test comment";
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.name = head_name;
    head.name_max = (uInt)(sizeof(head_name));
    head.comment = head_comment;
    head.comm_max = (uInt)(sizeof(head_comment));
    head.hcrc = 0;
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &head);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)read_bytes;
    long mark = inflateMark(&istrm);

    // step 4: Cleanup streams, close files and free buffers
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_r = gzclose(gr);
    delete [] read_buf;
    (void)rc_gzprintf;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}