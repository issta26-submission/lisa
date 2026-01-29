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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *num_new = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *item1 = NULL;
    char *printed = NULL;
    cJSON_bool was_number_before = 0;
    cJSON_bool replaced = 0;
    double replaced_value = 0.0;
    int derived_sum = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.75);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    was_number_before = cJSON_IsNumber(item1);
    num_new = cJSON_CreateNumber(99.99);
    replaced = cJSON_ReplaceItemInArray(retrieved_arr, 0, num_new);
    printed = cJSON_Print(root);
    replaced_value = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_arr, 0));
    derived_sum = (int)was_number_before + (int)replaced + (int)replaced_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}