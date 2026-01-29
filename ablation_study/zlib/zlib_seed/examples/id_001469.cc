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
//<ID> 1469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload: deflatePending/inflateUndermine/deflateSetHeader/gzungetc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    infstrm.next_in = src;
    infstrm.avail_in = (uInt)srcLen;

    // step 2: Setup (initialize streams and configure header)
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflateinit = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    const char name_str[] = "test-name";
    head.name_max = (uInt)(sizeof(name_str));
    head.name = new Bytef[(size_t)head.name_max];
    memset(head.name, 0, (size_t)head.name_max);
    memcpy(head.name, name_str, (size_t)(sizeof(name_str) - 1));
    int rc_setheader = deflateSetHeader(&defstrm, &head);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");

    // step 3: Core operations (operate and validate)
    unsigned int pending = 0u;
    int bits = 0;
    int rc_pending = deflatePending(&defstrm, &pending, &bits);
    int rc_undermine = inflateUndermine(&infstrm, 1);
    int rc_ungetc = gzungetc((int)src[0], gzf);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    int rc_inflateend = inflateEnd(&infstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] head.name;
    (void)version;
    (void)rc_definit;
    (void)rc_inflateinit;
    (void)rc_setheader;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)rc_undermine;
    (void)rc_ungetc;
    (void)rc_defend;
    (void)rc_inflateend;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}