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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "count", num);
    const char *colors_arr[] = { "red", "blue", "green" };
    cJSON *colors = cJSON_CreateStringArray(colors_arr, 3);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *s1 = cJSON_CreateString("delta");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "misc", arr);

    // step 2: Configure
    cJSON_SetNumberHelper(num, 255.5);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, s1);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON *colors_copy = cJSON_Duplicate(colors, 1);
    cJSON_AddItemToObject(root, "colors_copy", colors_copy);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    double count_value = cJSON_GetNumberValue(num);
    const char *det_str = cJSON_GetStringValue(detached);
    cJSON *dup_first_color = cJSON_GetArrayItem(colors_copy, 0);
    const char *first_color = cJSON_GetStringValue(dup_first_color);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = arr_size + (int)count_value + (int)det_str[0] + (int)first_color[0] + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}