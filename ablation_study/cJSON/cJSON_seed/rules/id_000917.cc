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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *strref = NULL;
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *retrieved_arr = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double first_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    strref = cJSON_CreateStringReference("hello world");
    n0 = cJSON_CreateNumber(42.0);
    n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(child, "text", strref);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItemCaseSensitive(root, "array");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_arr, 0));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "child");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)arr_size;
    (void)first_val;
    return 66;
}