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
//<ID> 1557
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
    cJSON *str_item0 = NULL;
    cJSON *str_item1 = NULL;
    cJSON *copied_item = NULL;
    cJSON *retrieved0 = NULL;
    cJSON *replacement = NULL;
    char *dup_buf = NULL;
    size_t dup_len = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    str_item0 = cJSON_CreateString("first");
    str_item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, str_item0);
    cJSON_AddItemToArray(arr, str_item1);

    // step 3: Operate / Validate
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    dup_len = strlen(cJSON_GetStringValue(retrieved0));
    dup_buf = (char *)cJSON_malloc(dup_len + 1);
    memcpy(dup_buf, cJSON_GetStringValue(retrieved0), dup_len + 1);
    copied_item = cJSON_AddStringToObject(root, "copied_first", dup_buf);
    (void)cJSON_Compare(retrieved0, copied_item, 1);
    replacement = cJSON_CreateString("replacement");
    (void)cJSON_ReplaceItemViaPointer(arr, retrieved0, replacement);

    // step 4: Cleanup
    cJSON_free(dup_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}