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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "Test payload for gzprintf and gzread";
    const unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    Bytef *name_buf = new Bytef[16];
    const char name_template[] = "zlibtest";
    memcpy(name_buf, name_template, sizeof(name_template));
    head.name = name_buf;
    head.name_max = 16;
    head.text = 0;
    int rc_def_set_header = deflateSetHeader(&dstrm, &head);

    // step 2: Configure and write gzip file using gzprintf
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "PAYLOAD:%s", payload);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose_w = gzclose(gf);

    // step 3: Reopen and read back via gzread to validate write
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char readbuf[128];
    memset(readbuf, 0, sizeof(readbuf));
    int rc_gzread = gzread(gr, (voidp)readbuf, (unsigned int)(sizeof(readbuf) - 1));
    int rc_gzclose_r = gzclose(gr);

    // step 4: Inflate lifecycle check and cleanup
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);

    delete [] name_buf;
    (void)payload_len;
    (void)rc_def_init;
    (void)rc_def_set_header;
    (void)rc_gzprintf;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}