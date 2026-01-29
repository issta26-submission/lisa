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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const str_vals[] = { "one", "two", "three" };
    cJSON *str_array = cJSON_CreateStringArray(str_vals, 3);
    float float_vals[] = { 1.0f, 2.5f, 3.75f };
    cJSON *num_array = cJSON_CreateFloatArray(float_vals, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON_AddItemToObject(root, "floats", num_array);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *str_dup = cJSON_Duplicate(str_array, 1);
    cJSON_AddItemToObject(root, "strings_copy", str_dup);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool equal_strings = cJSON_Compare(str_array, str_dup, 1);
    cJSON_bool equal_nums_self = cJSON_Compare(num_array, num_array, 1);
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));
    int summary = cJSON_GetArraySize(str_array) + (int)equal_strings + (int)equal_nums_self + (printed ? (int)printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}