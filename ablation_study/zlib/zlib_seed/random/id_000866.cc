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
//<ID> 866
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and write gz file
    const char src[] = "zlib api sequence: read with gzfread, crc checks, deflateInit2_, inflateReset";
    const z_size_t src_len = (z_size_t)(sizeof(src) - 1);
    const Bytef *src_buf = (const Bytef *)src;
    uLong crc_initial = crc32_z(0UL, src_buf, src_len);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, src, (unsigned int)src_len);
    int rc_gz_close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");

    // step 2: Initialize streams and buffers
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);
    z_size_t buf_size = src_len ? src_len : 1;
    Bytef *readBuf = new Bytef[buf_size];

    // step 3: Read from gzFile, compute CRCs, and attach to streams
    z_size_t items_read = gzfread((voidp)readBuf, 1, buf_size, gr);
    uInt items_u = (uInt)items_read;
    uLong crc_read = crc32(0UL, readBuf, items_u);
    uLong crc_combined = crc32_z(crc_initial, readBuf, items_read);
    dstrm.next_in = readBuf;
    dstrm.avail_in = items_u;
    istrm.adler = crc_combined;

    // step 4: Cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gz_close_r = gzclose(gr);
    delete [] readBuf;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_deflate_init2;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)crc_initial;
    (void)crc_read;
    (void)crc_combined;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gz_close_r;
    // API sequence test completed successfully
    return 66;
}