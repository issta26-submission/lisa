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
//<ID> 1573
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[3] = {5, 10, 15};
    cJSON *array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_array);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(child, "child_val", 2.5);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *second_dup_item = cJSON_GetArrayItem(dup_array, 1);
    double second_val = cJSON_GetNumberValue(second_dup_item);
    cJSON *computed = cJSON_CreateNumber(first_val + second_val);
    cJSON_AddItemToObject(root, "computed_sum", computed);
    cJSON *dup_first_shallow = cJSON_Duplicate(first_item, 0);
    cJSON_AddItemToObject(child, "dup_first", dup_first_shallow);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_DeleteItemFromObject(root, "numbers_copy");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}