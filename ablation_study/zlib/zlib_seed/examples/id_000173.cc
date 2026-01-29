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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: initialize inflate -> validate -> write gz -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)source;
    istrm.avail_in = (uInt)sourceLen;

    // step 2: Configure & Validate
    int rc_validate = inflateValidate(&istrm, 1);

    // step 3: Operate (open gz, write a byte, query error, close)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_putc = gzputc(gz, 'Z');
    int errnum = 0;
    const char *errstr = gzerror(gz, &errnum);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&istrm);

    (void)source;
    (void)sourceLen;
    (void)rc_inflate_init;
    (void)rc_validate;
    (void)rc_putc;
    (void)errnum;
    (void)errstr;
    (void)rc_gz_close;
    (void)rc_inflate_end;

    // API sequence test completed successfully
    return 66;
}