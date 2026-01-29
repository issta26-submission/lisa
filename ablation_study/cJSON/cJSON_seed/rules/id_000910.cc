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
//<ID> 910
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *strref = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_str = NULL;
    char *printed = NULL;
    const char *greeting_val = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    strref = cJSON_CreateStringReference("hello world");
    cJSON_AddItemToObject(root, "greeting", strref);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItemCaseSensitive(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_str = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    greeting_val = cJSON_GetStringValue(retrieved_str);
    printed = cJSON_PrintUnformatted(root);
    (void)greeting_val;
    (void)arr_size;
    (void)retrieved_arr;
    (void)retrieved_str;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "greeting");
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}