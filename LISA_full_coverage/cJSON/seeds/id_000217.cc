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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = {"alpha", "beta", "gamma"};
    cJSON *arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", arr);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "names_copy", dup_arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, first_item);
    cJSON_AddItemToObject(root, "first_detached", detached);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    double set_value = cJSON_SetNumberHelper(num_item, 123.456);
    cJSON_AddItemToArray(dup_arr, num_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON *parsed = cJSON_Parse(printed);
    int size_orig = cJSON_GetArraySize(arr);
    int size_dup = cJSON_GetArraySize(dup_arr);
    double num_val = cJSON_GetNumberValue(num_item);
    const char *det_str = cJSON_GetStringValue(detached);
    int summary = size_orig + size_dup + (int)num_val + (det_str ? (int)det_str[0] : 0) + root->type + (int)set_value + (parsed ? parsed->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}