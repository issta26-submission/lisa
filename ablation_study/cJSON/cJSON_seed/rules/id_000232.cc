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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *hello = cJSON_CreateString("hello");
    cJSON *world = cJSON_CreateString("world");
    cJSON *truev = cJSON_CreateTrue();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    const char *sval = NULL;
    cJSON *retrieved = NULL;
    cJSON *ref_item = NULL;
    cJSON_bool added_hello = 0;
    cJSON_bool added_true = 0;
    cJSON_bool ref_added = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    added_hello = cJSON_AddItemToArray(arr, hello);
    added_true = cJSON_AddItemToArray(arr, truev);
    ref_added = cJSON_AddItemReferenceToArray(arr, hello);
    cJSON_AddItemToArray(arr, world);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 0);
    sval = cJSON_GetStringValue(retrieved);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    ref_item = cJSON_GetArrayItem(arr, 2);
    (void)arr_size;
    (void)sval;
    (void)ref_item;
    (void)added_hello;
    (void)added_true;
    (void)ref_added;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}