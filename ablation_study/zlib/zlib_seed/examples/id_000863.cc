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
//<ID> 863
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and I/O
    const char payload[] = "zlib sequence payload for crc and gzfread";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef * payload_buf = (const Bytef *)payload;
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_puts = gzputs(gf_w, payload);
    int rc_gz_close_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef * readBuf = new Bytef[payload_len ? payload_len : 1];
    z_size_t items_read = gzfread((voidp)readBuf, (z_size_t)1, payload_len, gf_r);

    // step 2: CRC computations and stream initialization
    uLong crc_from_file = crc32(0UL, (const Bytef *)readBuf, (uInt)items_read);
    uLong crc_from_mem = crc32_z(0UL, payload_buf, payload_len);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);

    // step 3: Configure streams with data and validate via CRC
    dstrm.next_in = readBuf;
    dstrm.avail_in = (uInt)items_read;
    istrm.adler = (uLong)(crc_from_file ^ crc_from_mem);

    // step 4: Cleanup and finalize
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gz_close_r = gzclose(gf_r);
    delete [] readBuf;
    (void)payload_buf;
    (void)rc_gz_puts;
    (void)rc_gz_close_w;
    (void)items_read;
    (void)crc_from_file;
    (void)crc_from_mem;
    (void)rc_deflate_init2;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gz_close_r;
    // API sequence test completed successfully
    return 66;
}