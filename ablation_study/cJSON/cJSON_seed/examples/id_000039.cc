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
//<ID> 39
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
    cJSON_AddItemToObject(root, "list", array);

    // step 2: Configure
    cJSON *item_true = cJSON_CreateTrue();
    cJSON *item_str = cJSON_CreateString("initial");
    cJSON *item_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, item_true);
    cJSON_AddItemToArray(array, item_str);
    cJSON_AddItemToArray(array, item_num);
    cJSON *meta = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *replacement_false = cJSON_CreateFalse();
    cJSON_bool replaced0 = cJSON_ReplaceItemInArray(array, 0, replacement_false);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON *replacement_str = cJSON_CreateString("replaced");
    cJSON_bool replaced1 = cJSON_ReplaceItemInArray(array, 1, replacement_str);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    char *second_val = cJSON_GetStringValue(second);
    char *printed = cJSON_PrintUnformatted(root);
    (void)replaced0;
    (void)first_is_true;
    (void)replaced1;
    (void)second_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}