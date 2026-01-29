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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char payload[] = "zlib payload for gzputc and inflateValidate test\n";
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup & Configure
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)payload;
    istrm.avail_in = (uInt)(sizeof(payload) - 1);

    // step 3: Operate & Validate
    int rc_validate = inflateValidate(&istrm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int written1 = gzputc(gz, (int)payload[0]);
    int written2 = gzputc(gz, (int)payload[1]);
    int written3 = gzputc(gz, '\n');
    int errnum = 0;
    const char *err_msg = gzerror(gz, &errnum);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup & finalize
    int rc_inf_end = inflateEnd(&istrm);

    (void)rc_inf_init;
    (void)rc_validate;
    (void)written1;
    (void)written2;
    (void)written3;
    (void)errnum;
    (void)err_msg;
    (void)rc_gz_close;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}