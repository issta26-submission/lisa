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
//<ID> 1373
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *true_item = NULL;
    cJSON *another_true = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool equal = 0;
    double v0 = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(item0);
    another_true = cJSON_CreateTrue();
    equal = cJSON_Compare(true_item, another_true, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)v0;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(another_true);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}