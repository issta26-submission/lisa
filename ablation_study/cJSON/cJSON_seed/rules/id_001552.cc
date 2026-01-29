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
//<ID> 1552
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
    cJSON *retrieved = NULL;
    cJSON *dup_item = NULL;
    char *buffer = NULL;
    size_t buflen = 0;
    cJSON_bool compare_res = 0;
    cJSON_bool replace_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    item0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddStringToObject(root, "note", "created array with two strings");

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 0);
    buflen = (strlen(cJSON_GetStringValue(retrieved)) + 1);
    buffer = (char*)cJSON_malloc(buflen);
    memcpy(buffer, cJSON_GetStringValue(retrieved), buflen);
    dup_item = cJSON_CreateString(buffer);
    compare_res = cJSON_Compare(retrieved, dup_item, 1);
    replace_res = cJSON_ReplaceItemViaPointer(arr, retrieved, dup_item);

    // step 4: Cleanup
    cJSON_free(buffer);
    (void)compare_res;
    (void)replace_res;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}