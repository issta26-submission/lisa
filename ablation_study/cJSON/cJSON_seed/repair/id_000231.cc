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
//<ID> 231
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
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *str1 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(array, num0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddStringToObject(root, "category", "demo");

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "items");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_InsertItemInArray(retrieved, 1, false_item);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved, 0));
    cJSON_AddNumberToObject(root, "first_value_copy", first_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}