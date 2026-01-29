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
//<ID> 1163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *num_item1 = NULL;
    cJSON *num_item2 = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    char *dyn = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double val1 = 0.0;
    double val2 = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    num_item1 = cJSON_CreateNumber(42.5);
    num_item2 = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(array, num_item1);
    cJSON_AddItemToArray(array, num_item2);
    cJSON_AddItemToObject(root, "numbers", array);
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_valid", true_item);
    cJSON_AddItemToObject(root, "is_enabled", false_item);
    dyn = (char *)cJSON_malloc(16);
    memset(dyn, 0, 16);
    dyn[0] = 't'; dyn[1] = 'e'; dyn[2] = 's'; dyn[3] = 't'; dyn[4] = '\0';
    cJSON_AddStringToObject(root, "label", dyn);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(array);
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    val2 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 1));
    sum = val1 + val2;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(dyn);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}