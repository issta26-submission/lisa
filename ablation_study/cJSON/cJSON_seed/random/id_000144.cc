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
//<ID> 144
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
    cJSON *count_item = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *enabled_item = (cJSON *)0;
    char *json_text = (char *)0;
    int array_size = 0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    cJSON_bool add_result = (cJSON_bool)0;
    double validated_count = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(12.0);
    num2 = cJSON_CreateNumber(3.14);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    add_result = cJSON_AddItemToObjectCS(root, "values", arr);
    enabled_item = cJSON_AddTrueToObject(root, "enabled");
    array_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)array_size);
    add_result = cJSON_AddItemToObjectCS(root, "count_cs", count_item);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_is_number = cJSON_IsNumber(first_item);
    cJSON_AddNumberToObject(root, "first_is_number", (double)first_is_number);

    // step 4: Operate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 5: Validate
    validated_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count_cs"));
    cJSON_AddNumberToObject(root, "validated_count", validated_count);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}