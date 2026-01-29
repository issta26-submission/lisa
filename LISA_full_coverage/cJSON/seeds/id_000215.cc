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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const words[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", str_array);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "initial", num_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "inner_number", cJSON_CreateNumber(1.5));
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    double set_result = cJSON_SetNumberHelper(num_item, 84.0);
    (void)set_result;
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", child_dup);
    cJSON *second = cJSON_GetArrayItem(str_array, 1);
    cJSON *detached = cJSON_DetachItemViaPointer(str_array, second);
    cJSON_AddItemToObject(root, "detached_word", detached);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON *parsed = cJSON_Parse(printed);
    int array_size = cJSON_GetArraySize(str_array);
    cJSON *initial = cJSON_GetObjectItem(root, "initial");
    double initial_val = cJSON_GetNumberValue(initial);
    char *det_str = cJSON_GetStringValue(detached);
    int summary = array_size + (int)initial_val + (int)det_str[0] + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}