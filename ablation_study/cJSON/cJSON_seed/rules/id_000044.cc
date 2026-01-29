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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *label = cJSON_CreateString("example_set");
    cJSON *dup = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    char *printed = NULL;
    int count = 0;
    double sum = 0.0;
    cJSON_bool equal_copy = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    count = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    sum = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "sum", sum);
    dup = cJSON_Duplicate(root, 1);
    equal_copy = cJSON_Compare(root, dup, 1);
    cJSON_AddBoolToObject(root, "equal_copy", equal_copy);
    printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}