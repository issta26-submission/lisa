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
//<ID> 1376
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *true_item = NULL;
    cJSON *item0 = NULL;
    cJSON *item2 = NULL;
    char *printed = NULL;
    int size = 0;
    cJSON_bool same = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)size);
    item0 = cJSON_GetArrayItem(arr, 0);
    item2 = cJSON_GetArrayItem(arr, 2);
    same = cJSON_Compare(item2, true_item, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)item0;
    (void)same;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}