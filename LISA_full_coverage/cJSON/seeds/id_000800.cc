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
//<ID> 800
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON *id = cJSON_CreateNumber(2026.0);
    cJSON *name = cJSON_CreateString("cJSON_test");

    // step 2: Configure
    cJSON_AddItemToObject(meta, "flag", flag);
    cJSON_AddItemToObject(meta, "id", id);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddRawToObject(root, "raw_payload", "{\"embedded\":true,\"count\":5}");

    // step 3: Operate and Validate
    cJSON *meta_lookup = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *flag_lookup = cJSON_GetObjectItemCaseSensitive(meta_lookup, "flag");
    cJSON_bool flag_value = cJSON_IsTrue(flag_lookup);
    cJSON *flag_number = cJSON_CreateNumber((double)flag_value);
    cJSON_AddItemToObject(root, "flag_value", flag_number);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}