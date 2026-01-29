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
//<ID> 1401
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
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *label = NULL;
    cJSON_bool prealloc_ok = 0;
    char *buffer = NULL;
    int buffer_len = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(child, "list", arr);
    cJSON_AddItemToObject(root, "child", child);
    label = cJSON_AddStringToObject(root, "label", "demo");
    cJSON_AddItemReferenceToObject(root, "child_ref", child);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    buffer_len = 128 + arr_size * 32;
    buffer = (char *)cJSON_malloc((size_t)buffer_len);
    memset(buffer, 0, (size_t)buffer_len);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, buffer_len, 1);
    (void)prealloc_ok;
    (void)label;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}