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
//<ID> 370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *value = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "value", value);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_missing", maybe_missing);

    // step 2: Configure
    cJSON *greeting_dup = cJSON_Duplicate(greeting, 1);
    cJSON_AddItemToObject(root, "greeting_dup", greeting_dup);
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    double num_val = cJSON_GetNumberValue(value);

    // step 3: Operate & Validate
    char *buf = (char *)cJSON_malloc(128);
    memset(buf, 0, 128);
    sprintf(buf, "%s|%.3f|null=%d", cJSON_GetStringValue(greeting_dup), num_val, (int)was_null);
    cJSON *snapshot = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "snapshot", snapshot);
    cJSON_free(buf);
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON *dump = cJSON_CreateString(unfmt);
    cJSON_AddItemToObject(root, "dump", dump);
    cJSON_free(unfmt);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}