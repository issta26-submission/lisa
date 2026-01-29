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
//<ID> 864
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial data
    const char filename[] = "tmp_zlib_api_sequence.gz";
    const char payload[] = "zlib test payload for crc and gz I/O";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef * payload_buf = (const Bytef *)payload;
    uLong crc_payload = crc32_z(0UL, payload_buf, payload_len);

    // step 2: Write payload to a gzip file and initialize streams
    gzFile gw = gzopen(filename, "wb");
    int rc_gz_write = gzwrite(gw, (voidp)(Bytef *)payload_buf, (unsigned int)payload_len);
    int rc_gz_close_w = gzclose(gw);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);

    // step 3: Read back the gzip file, compute CRCs and propagate into streams
    gzFile gr = gzopen(filename, "rb");
    Bytef * readBuf = new Bytef[payload_len ? payload_len : 1];
    memset(readBuf, 0, payload_len ? payload_len : 1);
    z_size_t items_read = gzfread((voidp)readBuf, (z_size_t)1, payload_len, gr);
    uLong crc_read = crc32(0UL, readBuf, (uInt)items_read);
    uLong crc_read_z = crc32_z(0UL, readBuf, items_read);
    dstrm.adler = crc_read;
    istrm.adler = crc_read_z;

    // step 4: Cleanup and finalize
    int rc_gz_close_r = gzclose(gr);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] readBuf;
    (void)crc_payload;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)items_read;
    (void)crc_read;
    (void)crc_read_z;
    (void)rc_gz_close_r;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}