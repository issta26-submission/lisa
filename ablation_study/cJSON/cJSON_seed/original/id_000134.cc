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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *strings[] = { "one", "two", "three" };
    cJSON *str_arr = cJSON_CreateStringArray(strings, 3);
    float floats[] = { 1.0f, 2.5f };
    cJSON *float_arr = cJSON_CreateFloatArray(floats, 2);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "strings", str_arr);
    cJSON_AddItemToObject(root, "floats", float_arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "note", "configured");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *dup_str_arr = cJSON_Duplicate(str_arr, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool same = cJSON_Compare(str_arr, dup_str_arr, 1);
    int summary = cJSON_GetArraySize(str_arr) + cJSON_GetArraySize(float_arr) + (int)same + (printed ? (int)printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(dup_str_arr);

    // API sequence test completed successfully
    return 66;
}