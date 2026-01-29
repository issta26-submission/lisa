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
//<ID> 899
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib payload for gzprintf/gzread, compress2, deflate header and inflateMark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong crc_src = crc32(0UL, src_buf, (uInt)payload_len);
    uLong bound = compressBound((uLong)payload_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);

    // step 2: Write payload to a gzip file using gzprintf, then read it back using gzread
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzprintf = gzprintf(gf, "%s", payload);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);

    // step 3: Initialize deflate stream, set a gzip header, initialize inflate stream and get its mark
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    static Bytef head_name[] = "example_name";
    head.name = head_name;
    head.name_max = (uInt)(sizeof(head_name) - 1);
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &head);
    int rc_deflate_end = deflateEnd(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long inflate_mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gf);
    delete [] read_buf;
    delete [] comp_buf;
    (void)crc_src;
    (void)rc_compress;
    (void)crc_read;
    (void)rc_gzprintf;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)inflate_mark;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}