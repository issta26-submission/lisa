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
//<ID> 814
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta_raw = NULL;
    cJSON *flag_false = NULL;
    cJSON *dup_root = NULL;
    cJSON *picked = NULL;
    char buffer[256];
    int buf_len = 256;
    const char *raw_text = "{\"meta\":true,\"count\":3}";
    cJSON_bool printed = 0;
    cJSON_bool equal = 0;
    memset(buffer, 0, buf_len);

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta_raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "meta_raw", meta_raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag_false);

    // step 3: Operate / Validate
    printed = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    picked = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    dup_root = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)printed;
    (void)picked;
    (void)equal;
    return 66;
}