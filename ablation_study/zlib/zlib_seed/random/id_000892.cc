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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for gzprintf/gzread and stream header/mark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    Bytef *name_buf = new Bytef[32];
    memset(name_buf, 0, (size_t)32);
    // copy a short name into name_buf without loops
    name_buf[0] = 's'; name_buf[1] = 'e'; name_buf[2] = 'q'; name_buf[3] = '\0';

    gz_header gzhead;
    memset(&gzhead, 0, (size_t)sizeof(gz_header));
    gzhead.name = name_buf;
    gzhead.name_max = 32;
    gzhead.text = 1;

    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize and configure deflate stream with gzip header
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &gzhead);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Operate: write payload via gzprintf, then read it back via gzread
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gw, payload);
    int rc_gzflush = gzflush(gw, 0);
    int rc_gzclose_w = gzclose_w(gw);

    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gr, (voidp)read_buf, to_read);
    int rc_gzclose_r = gzclose_r(gr);

    // step 4: Validate using inflate lifecycle and mark, then cleanup
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)to_read;
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    delete [] read_buf;
    delete [] name_buf;

    (void)src_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate_end;
    (void)rc_gzprintf;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}