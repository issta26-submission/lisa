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
//<ID> 1467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for header/pending/undermine/gzungetc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream instrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    gz_header head;
    memset(&head, 0, sizeof(head));
    Bytef *gname = new Bytef[16];
    memset(gname, 0, 16);
    const char gname_text[] = "test_name";
    memcpy(gname, gname_text, (size_t)(sizeof(gname_text) - 1));
    head.name = gname;
    head.name_max = (uInt)(sizeof(gname_text) - 1);

    // step 2: Setup
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_head = deflateSetHeader(&defstrm, &head);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int rc_pending = deflatePending(&defstrm, &pending_bytes, &pending_bits);
    int rc_inf_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_undermine = inflateUndermine(&instrm, 1);

    // step 3: Core operations
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_put = gzputc(gzf_w, (int)payload[0]);
    int rc_close_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c_read = gzgetc(gzf_r);
    int rc_unget = gzungetc(c_read, gzf_r);
    int rc_close_r = gzclose(gzf_r);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&instrm);
    delete [] src;
    delete [] gname;
    (void)version;
    (void)rc_def_init;
    (void)rc_set_head;
    (void)pending_bytes;
    (void)pending_bits;
    (void)rc_pending;
    (void)rc_inf_init;
    (void)rc_undermine;
    (void)rc_put;
    (void)rc_close_w;
    (void)c_read;
    (void)rc_unget;
    (void)rc_close_r;
    // API sequence test completed successfully
    return 66;
}