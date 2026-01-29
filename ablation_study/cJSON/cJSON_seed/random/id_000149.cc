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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *count_num = (cJSON *)0;
    cJSON *first_is_num_flag = (cJSON *)0;
    cJSON *size_after_num = (cJSON *)0;
    cJSON *always_true = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    values = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "values", values);

    // step 4: Operate
    size_before = cJSON_GetArraySize(values);
    count_num = cJSON_CreateNumber((double)size_before);
    cJSON_AddItemToObjectCS(meta, "initial_count", count_num);
    first_is_number = cJSON_IsNumber(cJSON_GetArrayItem(values, 0));
    first_is_num_flag = cJSON_CreateNumber((double)first_is_number);
    cJSON_AddItemToObjectCS(meta, "first_is_number", first_is_num_flag);
    always_true = cJSON_AddTrueToObject(meta, "validated_true");

    // step 5: Validate
    size_after = cJSON_GetArraySize(values);
    size_after_num = cJSON_CreateNumber((double)size_after);
    cJSON_AddItemToObjectCS(root, "size_after", size_after_num);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}