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
//<ID> 1406
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
    cJSON *item_str = NULL;
    cJSON *item_num = NULL;
    cJSON *retrieved = NULL;
    char *buffer = NULL;
    cJSON_bool printed_ok = 0;
    int arr_size = 0;
    const char *retrieved_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    item_str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, item_str);
    item_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, item_num);
    cJSON_AddItemReferenceToObject(root, "str_ref", item_str);
    cJSON_AddStringToObject(root, "direct", "world");

    // step 3: Operate / Validate
    buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    arr_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetObjectItem(root, "str_ref");
    retrieved_str = cJSON_GetStringValue(retrieved);
    (void)printed_ok;
    (void)arr_size;
    (void)retrieved_str;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}