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
//<ID> 1158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char *version = zlibVersion();
    z_stream istrm;
    z_stream istrm_copy;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    gz_header gh;
    memset(&gh, 0, (size_t)sizeof(gz_header));
    gh.name = (Bytef *)"zlib_example_name";
    gh.name_max = (uInt)16;
    gh.comment = (Bytef *)"zlib_example_comment";
    gh.comm_max = (uInt)20;

    // step 2: Setup (initialize inflate with windowBits and initialize deflate to accept header)
    int rc_inflate_init2 = inflateInit2_(&istrm, 15 + 32, version, (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &gh);

    // step 3: Core operations (open gz files and copy inflate state)
    gzFile gz1 = gzopen("zlib_api_seq_1.gz", "wb");
    gzFile gz2 = gzopen64("zlib_api_seq_2.gz", "wb");
    int rc_gzputs1 = gzputs(gz1, "test_payload_one");
    int rc_gzputs2 = gzputs(gz2, "test_payload_two");
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    int rc_gzclose1 = gzclose(gz1);
    int rc_gzclose2 = gzclose(gz2);
    (void)version;
    (void)gh;
    (void)rc_inflate_init2;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_gzputs1;
    (void)rc_gzputs2;
    (void)rc_inflate_copy;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    // API sequence test completed successfully
    return 66;
}