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
//<ID> 257
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
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *n1 = cJSON_CreateNumber(1.23);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON *b_insert = cJSON_CreateTrue();
    
    // step 2: Configure
    cJSON_InsertItemInArray(arr, 1, b_insert); // insert true at index 1 -> [1.23, true, false]
    cJSON_AddNumberToObject(root, "initial_count", (double)cJSON_GetArraySize(arr));
    
    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool item1_is_bool = cJSON_IsBool(item1);
    cJSON_bool item1_is_true = cJSON_IsTrue(item1);
    cJSON_AddNumberToObject(root, "item1_is_bool", (double)item1_is_bool);
    cJSON_AddNumberToObject(root, "item1_is_true", (double)item1_is_true);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = size + (int)item1_is_bool + (int)item1_is_true + (int)printed[0];
    (void)summary;
    
    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    
    // API sequence test completed successfully
    return 66;
}