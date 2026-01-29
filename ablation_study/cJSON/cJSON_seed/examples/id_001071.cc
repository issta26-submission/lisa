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
//<ID> 1071
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
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(root, "name", "example_name");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    int size = cJSON_GetArraySize(got_list);
    cJSON *count_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count_num);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_bool = cJSON_IsBool(got_active);
    cJSON_AddStringToObject(root, "echo", name_val);
    cJSON_AddBoolToObject(root, "active_copy", is_bool);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}