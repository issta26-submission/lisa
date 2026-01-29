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
//<ID> 484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num2);
    cJSON *label = cJSON_CreateString("values_array");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *computed = cJSON_CreateNumber(second_val * 2.0);
    cJSON_AddItemToObject(root, "computed", computed);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}