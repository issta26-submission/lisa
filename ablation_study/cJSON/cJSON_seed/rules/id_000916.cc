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
//<ID> 916
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *str_ref = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_child = NULL;
    const char *key_items = "items";
    const char *key_name = "name";
    const char *name_value = "example";
    const char *got_name = NULL;
    char *copied_name = NULL;
    size_t name_len = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    str_ref = cJSON_CreateStringReference(name_value);
    cJSON_AddItemToObject(child, key_name, str_ref);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, key_items, arr);
    num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItemCaseSensitive(root, key_items);
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_child = cJSON_GetObjectItemCaseSensitive(root, "child");
    got_name = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(retrieved_child, key_name));
    name_len = (got_name != NULL) ? strlen(got_name) : 0;
    copied_name = (char *)cJSON_malloc(name_len + 1);
    if (copied_name) {
        if (got_name && name_len) {
            memcpy(copied_name, got_name, name_len);
        }
        if (copied_name) {
            copied_name[name_len] = '\0';
        }
    }
    cJSON_DeleteItemFromObjectCaseSensitive(retrieved_child, key_name);

    // step 4: Cleanup
    if (copied_name) {
        cJSON_free(copied_name);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)arr_size;
    (void)got_name;
    return 66;
}