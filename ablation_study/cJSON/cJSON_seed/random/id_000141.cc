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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *tmp = (cJSON *)0;
    char *json_text = (char *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    cJSON_bool second_is_number = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObjectCS(root, "values", arr);
    tmp = cJSON_AddTrueToObject(root, "active");

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    first_is_number = cJSON_IsNumber(cJSON_GetArrayItem(arr, 0));
    second_is_number = cJSON_IsNumber(cJSON_GetArrayItem(arr, 1));
    tmp = cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    tmp = cJSON_AddNumberToObject(root, "first_is_number", (double)first_is_number);
    tmp = cJSON_AddNumberToObject(root, "second_is_number", (double)second_is_number);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);
    size_after = cJSON_GetArraySize(arr);
    tmp = cJSON_AddNumberToObject(root, "size_after", (double)size_after);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}