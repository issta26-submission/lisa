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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const words[3] = { "one", "two", "three" };
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_array = cJSON_CreateStringArray(words, 3);
    cJSON *solo_str = cJSON_CreateString("solo");
    cJSON_AddItemToArray(arr, str_array);
    cJSON_AddItemToArray(arr, solo_str);
    cJSON_AddItemToObject(root, "nested", arr);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(1.0);
    double updated_num = cJSON_SetNumberHelper(num, 42.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON *replacement = cJSON_CreateNumber(7.7);
    cJSON_ReplaceItemInArray(arr, 0, replacement);

    // step 3: Operate & Validate
    double rep_val = cJSON_GetNumberValue(replacement);
    cJSON *sum = cJSON_CreateNumber(updated_num + rep_val);
    cJSON_AddItemToObject(root, "sum", sum);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}