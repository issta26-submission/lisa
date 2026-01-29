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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    char *printed = (char *)0;
    float numbers[4];
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    numbers[0] = 1.1f;
    numbers[1] = 2.2f;
    numbers[2] = 3.3f;
    numbers[3] = 4.4f;
    float_array = cJSON_CreateFloatArray(numbers, 4);
    str_item = cJSON_CreateString("created_string");

    // step 3: Configure
    cJSON_AddItemToObject(root, "float_values", float_array);
    cJSON_AddItemToObject(root, "manual_string", str_item);
    cJSON_AddStringToObject(root, "inline_string", "inline_value");
    cJSON_AddNumberToObject(root, "array_len_placeholder", 0.0);

    // step 4: Operate & Validate
    retrieved_array = cJSON_GetObjectItem(root, "float_values");
    arr_size = cJSON_GetArraySize(retrieved_array);
    cJSON_AddNumberToObject(root, "array_len", (double)arr_size);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}