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
//<ID> 839
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

static unsigned int my_in_func(void *in_desc, unsigned char **buf) {
    z_stream *s = (z_stream *)in_desc;
    *buf = s->next_in;
    return s->avail_in;
}

static int my_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    z_stream *s = (z_stream *)out_desc;
    memcpy(s->next_out, buf, len);
    s->next_out += len;
    s->avail_out -= len;
    s->total_out += len;
    return 0;
}

int test_zlib_api_sequence() {
    const char src[] = "zlib api sequence payload for gzwrite, gzoffset64, inflateBack and crc32";
    const unsigned int srcLen = (unsigned int)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_initial = crc32(0L, source, (uInt)srcLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");

    int rc_gz_write = gzwrite(gf, source, (unsigned int)srcLen);
    off64_t offset_after_write = gzoffset64(gf);

    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)srcLen;

    static unsigned char window[(1 << 15)];
    int rc_inflate_back_init = inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    unsigned char outbuf[65536];
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);

    int rc_inflate_back = inflateBack(&strm, my_in_func, (void *)&strm, my_out_func, (void *)&strm);
    int rc_inflate_back_end = inflateBackEnd(&strm);

    int rc_close = gzclose(gf);

    (void)crc_initial;
    (void)rc_gz_write;
    (void)offset_after_write;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back;
    (void)rc_inflate_back_end;
    (void)rc_close;

    return 66;
}