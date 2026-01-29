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
//<ID> 578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize streams and allocate window for back-inflate
    const char *version = zlibVersion();
    z_stream istrm1;
    memset(&istrm1, 0, sizeof(z_stream));
    int rc_init1 = inflateInit_(&istrm1, version, (int)sizeof(z_stream));
    z_stream istrm2;
    memset(&istrm2, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_back_init = inflateBackInit_(&istrm2, 15, window, version, (int)sizeof(z_stream));

    // step 2: Operate on a gzFile: write payload and obtain current file position
    const char payload[] = "zlib API payload: inflateInit_ -> inflateBackInit_ -> gzwrite -> gztell";
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidp)payload, (unsigned int)(sizeof(payload) - 1));
    off_t current_pos = gztell(gf);

    // step 3: Configure/validate by propagating gztell result into stream metadata and resetting
    istrm1.total_in = (uLong)current_pos;
    istrm1.total_out = (uLong)current_pos;
    int rc_reset_keep = inflateResetKeep(&istrm1);

    // step 4: Cleanup resources and finalize
    int rc_end1 = inflateEnd(&istrm1);
    int rc_back_end = inflateBackEnd(&istrm2);
    int rc_gzclose = gzclose(gf);
    free(window);
    (void)version;
    (void)rc_init1;
    (void)rc_back_init;
    (void)rc_gzwrite;
    (void)current_pos;
    (void)rc_reset_keep;
    (void)rc_end1;
    (void)rc_back_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}