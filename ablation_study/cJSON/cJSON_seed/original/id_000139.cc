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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *strings[] = { "first", "second", "third" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    float numbers[] = { 1.5f, -2.5f, 3.0f };
    cJSON *float_array = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON_AddItemToObject(root, "numbers", float_array);

    // step 2: Configure
    cJSON *str_copy = cJSON_Duplicate(str_array, 1);
    cJSON *num_copy = cJSON_Duplicate(float_array, 1);
    cJSON_AddItemToObject(root, "strings_copy", str_copy);
    cJSON_AddItemToObject(root, "numbers_copy", num_copy);

    // step 3: Operate and Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_bool equal1 = cJSON_Compare(str_array, str_copy, 1);
    cJSON_bool equal2 = cJSON_Compare(float_array, num_copy, 1);
    int size_str = cJSON_GetArraySize(str_array);
    int size_num = cJSON_GetArraySize(float_array);
    int summary = (int)equal1 + (int)equal2 + size_str + size_num + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}