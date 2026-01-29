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
//<ID> 678
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *data = cJSON_AddObjectToObject(root, "data");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "title", "example");
    cJSON_AddNumberToObject(meta, "version", 1.5);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(data, "values", arr);

    // step 3: Operate and Validate
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_str = cJSON_IsString(first);
    cJSON_AddBoolToObject(meta, "first_is_string", first_is_str);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "snapshot", snapshot);
    cJSON_free(snapshot);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}