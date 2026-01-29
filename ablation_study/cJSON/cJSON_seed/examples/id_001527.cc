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
//<ID> 1527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON *label = cJSON_CreateString("example");
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_bool added_values = cJSON_AddItemToObject(root, "values", array);
    cJSON_bool added_label = cJSON_AddItemToObject(root, "label", label);
    cJSON_bool added_flag = cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON *item1 = cJSON_GetArrayItem(array, 1);
    cJSON_bool item0_is_num = cJSON_IsNumber(item0);
    cJSON_bool item1_is_num = cJSON_IsNumber(item1);
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    cJSON_bool label_is_string = cJSON_IsString(label_item);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    (void)added_values;
    (void)added_label;
    (void)added_flag;
    (void)replaced;
    (void)item0_is_num;
    (void)item1_is_num;
    (void)label_is_string;
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}