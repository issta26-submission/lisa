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
//<ID> 801
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
    cJSON *raw = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool items_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    items_equal = cJSON_Compare(item0, item1, 0);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    cJSON_AddBoolToObject(root, "items_equal", items_equal);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}