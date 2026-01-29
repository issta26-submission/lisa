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
//<ID> 1558
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *replacement = NULL;
    cJSON *cmp_bool = NULL;
    cJSON *retrieved0 = NULL;
    cJSON *retrieved1 = NULL;
    char *heap_key = NULL;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    item0 = cJSON_CreateString("first");
    item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 3: Operate / Validate
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(arr, item1, replacement);
    retrieved1 = cJSON_GetArrayItem(arr, 1);
    heap_key = (char*)cJSON_malloc(8);
    heap_key[0] = 'd'; heap_key[1] = 'y'; heap_key[2] = 'n'; heap_key[3] = 'a';
    heap_key[4] = 'm'; heap_key[5] = 'i'; heap_key[6] = 'c'; heap_key[7] = '\0';
    cJSON_AddStringToObject(root, heap_key, "inserted");
    cmp_result = cJSON_Compare(retrieved0, retrieved1, 0);
    cmp_bool = cJSON_CreateBool(cmp_result);
    cJSON_AddItemToObject(root, "comparison", cmp_bool);

    // step 4: Cleanup
    cJSON_free(heap_key);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}