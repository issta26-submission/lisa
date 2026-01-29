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
//<ID> 1462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflatePending/inflateUndermine/deflateSetHeader/gzungetc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));

    // step 2: Setup
    defStrm.next_in = src;
    defStrm.avail_in = (uInt)srcLen;
    const uInt outBufSize = 512u;
    Bytef *outBuf = new Bytef[(size_t)outBufSize];
    memset(outBuf, 0, (size_t)outBufSize);
    defStrm.next_out = outBuf;
    defStrm.avail_out = outBufSize;
    int rc_def_init = deflateInit_(&defStrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&infStrm, version, (int)sizeof(z_stream));

    // step 3: Core operations
    gz_header head;
    memset(&head, 0, sizeof(head));
    head.text = 0;
    head.time = 0;
    head.name = (Bytef *)0;
    int rc_set_hdr = deflateSetHeader(&defStrm, &head);
    unsigned int pending = 0u;
    int bits = 0;
    int rc_pending = deflatePending(&defStrm, &pending, &bits);
    int rc_undermine = inflateUndermine(&infStrm, 1);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_ungetc = gzungetc((int)payload[0], gzf);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&defStrm);
    int rc_inf_end = inflateEnd(&infStrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] outBuf;
    (void)version;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_set_hdr;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)rc_undermine;
    (void)rc_ungetc;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}