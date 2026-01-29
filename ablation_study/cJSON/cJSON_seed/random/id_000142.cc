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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *json_text = (char *)0;
    int arr_size = 0;
    cJSON_bool second_is_number = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(7.5);

    // step 3: Configure
    cJSON_AddItemToObjectCS(root, "measurements", arr);
    cJSON_AddItemToObjectCS(root, "meta", child);
    cJSON_AddTrueToObject(child, "valid");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 1);
    second_is_number = cJSON_IsNumber(retrieved);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    cJSON_AddNumberToObject(root, "second_is_number", (double)second_is_number);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}