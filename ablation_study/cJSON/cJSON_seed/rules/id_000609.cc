#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *printed_raw = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_Parse("{}");
    cJSON *raw = cJSON_CreateRaw("{\"note\":\"raw\",\"value\":123}");
    size_t root_len = 0;
    size_t raw_len = 0;
    size_t to_copy_root = 0;
    size_t to_copy_raw = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate / Validate
    printed_root = cJSON_PrintUnformatted(root);
    printed_raw = cJSON_PrintUnformatted(raw);
    root_len = printed_root ? strlen(printed_root) : 0;
    raw_len = printed_raw ? strlen(printed_raw) : 0;
    to_copy_root = root_len < 255 ? root_len : 255;
    to_copy_raw = (raw_len < (511 - to_copy_root)) ? raw_len : (511 - to_copy_root);
    memcpy(scratch, printed_root ? printed_root : "", to_copy_root);
    scratch[to_copy_root] = '\0';
    memcpy(scratch + to_copy_root, printed_raw ? printed_raw : "", to_copy_raw);
    scratch[to_copy_root + to_copy_raw] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_raw);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(raw);

    // API sequence test completed successfully
    return 66;
}