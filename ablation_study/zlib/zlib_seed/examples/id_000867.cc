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
//<ID> 867
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial CRC computations
    const char payload[] = "zlib api sequence payload";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *pbuf = (const Bytef *)payload;
    uLong crc_initial = crc32(0UL, pbuf, (uInt)payload_len);
    uLong crc_initial_z = crc32_z(0UL, pbuf, payload_len);

    // step 2: Initialize zlib streams (deflate and inflate) and reset inflate
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);

    // step 3: Write payload to a gz file, then read it back with gzfread
    const char fname[] = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int rc_gz_write = gzwrite(gw, (voidpc)pbuf, (unsigned int)payload_len);
    int rc_gz_flush = gzflush(gw, 0);
    int rc_gz_close_w = gzclose_w ? gzclose_w(gw) : gzclose(gw);
    (void)rc_gz_flush;
    (void)rc_gz_write;
    (void)rc_gz_close_w;

    gzFile gr = gzopen(fname, "rb");
    Bytef *readBuf = new Bytef[payload_len ? payload_len : 1];
    z_size_t items_read = gzfread((voidp)readBuf, (z_size_t)1, payload_len, gr);
    int rc_gz_close_r = gzclose(gr);
    (void)rc_gz_close_r;

    // step 4: Validation and cleanup
    uLong crc_read_z = crc32_z(0UL, readBuf, items_read);
    uLong crc_combined = crc32(crc_initial, readBuf, (uInt)items_read);
    istrm.next_in = readBuf;
    istrm.avail_in = (uInt)items_read;
    istrm.total_in = 0;
    istrm.adler = crc_read_z;
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] readBuf;
    (void)crc_initial;
    (void)crc_initial_z;
    (void)crc_read_z;
    (void)crc_combined;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}