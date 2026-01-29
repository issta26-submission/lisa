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
//<ID> 168
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
    cJSON_AddItemToObject(root, "values", array);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(array, num1);
    cJSON *num2 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num2, 42.0);
    cJSON_AddItemToArray(array, num2);
    cJSON *str = cJSON_CreateString("reference-string");
    cJSON_AddItemReferenceToArray(array, str);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(array);
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    double val0 = cJSON_GetNumberValue(item0);
    cJSON *item1 = cJSON_GetArrayItem(array, 1);
    double val1 = cJSON_GetNumberValue(item1);
    cJSON *item2 = cJSON_GetArrayItem(array, 2);
    char *strval = cJSON_GetStringValue(item2);
    cJSON_AddBoolToObject(root, "first_is_number", cJSON_IsNumber(item0));
    cJSON_AddBoolToObject(root, "second_is_number", cJSON_IsNumber(item1));
    cJSON_AddBoolToObject(root, "third_is_string", cJSON_IsString(item2));
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddNumberToObject(root, "sum01", val0 + val1);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}