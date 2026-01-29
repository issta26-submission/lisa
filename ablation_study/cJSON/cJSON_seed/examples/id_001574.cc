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
//<ID> 1574
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
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.5);
    cJSON *num2 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(array, num0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "values", array);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "values_copy", dup_array);
    cJSON_AddNumberToObject(root, "scale", 1.5);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON_DeleteItemFromObject(root, "label");
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}