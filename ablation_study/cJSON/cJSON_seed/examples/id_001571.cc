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
//<ID> 1571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {1, 2, 3, 4};
    cJSON *array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "nums", array);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNumberToObject(root, "version", 2.5);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "nums_copy", dup_array);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *second_dup_item = cJSON_GetArrayItem(dup_array, 1);
    double second_dup_val = cJSON_GetNumberValue(second_dup_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    double flag_val = (double)cJSON_IsTrue(flag_item);
    cJSON *computed = cJSON_CreateNumber(first_val + second_dup_val + flag_val);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON_DeleteItemFromObject(root, "name");
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}