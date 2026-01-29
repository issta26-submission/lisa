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
//<ID> 1377
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
    cJSON *elem = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num_compare = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    int sz = 0;
    double value = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);
    num2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddNumberToObject(root, "total_expected", 60.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    sz = cJSON_GetArraySize(arr);
    elem = cJSON_GetArrayItem(arr, 1);
    value = cJSON_GetNumberValue(elem);
    num_compare = cJSON_CreateNumber(value);
    equal = cJSON_Compare(elem, num_compare, 1);
    cJSON_AddNumberToObject(root, "extracted", value);
    printed = cJSON_PrintUnformatted(root);
    (void)sz;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(num_compare);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}