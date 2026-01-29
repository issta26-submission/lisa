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
//<ID> 933
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *name = cJSON_CreateString("device-alpha");
    cJSON *raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON *num = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.5));
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "raw_payload", raw);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate & Validate
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON *got_name = cJSON_GetObjectItem(copy, "name");
    cJSON_bool name_is_str = cJSON_IsString(got_name);
    cJSON_AddBoolToObject(copy, "name_is_string", name_is_str);
    cJSON *original_raw = cJSON_GetObjectItem(root, "raw_payload");
    cJSON *copied_raw = cJSON_Duplicate(original_raw, 0);
    cJSON_AddItemToObject(copy, "raw_shallow_copy", copied_raw);
    char *snapshot = cJSON_PrintUnformatted(copy);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(copy);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}