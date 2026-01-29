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
//<ID> 1521
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(values, str_item);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_bool added_flag = cJSON_AddItemToObject(root, "flag", flag);
    cJSON_bool added_values = cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(values, 1, replacement);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_bool added_answer = cJSON_AddItemToObject(root, "answer", answer);

    // step 3: Operate & Validate
    cJSON *item_at_1 = cJSON_GetArrayItem(values, 1);
    cJSON_bool item_is_string = cJSON_IsString(item_at_1);
    cJSON *answer_item = cJSON_GetObjectItem(root, "answer");
    cJSON_bool answer_is_number = cJSON_IsNumber(answer_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)added_flag;
    (void)added_values;
    (void)replaced;
    (void)added_answer;
    (void)item_is_string;
    (void)answer_is_number;
    (void)item_at_1;
    (void)answer_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}