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
//<ID> 1555
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
    cJSON *s0 = NULL;
    cJSON *s1 = NULL;
    cJSON *replacement = NULL;
    cJSON *got0 = NULL;
    const char *got_str = NULL;
    char *name_buf = NULL;
    size_t name_len = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    s0 = cJSON_CreateString("first");
    s1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);

    // step 3: Operate / Validate
    got0 = cJSON_GetArrayItem(arr, 0);
    got_str = cJSON_GetStringValue(got0);
    name_len = (got_str ? strlen(got_str) : 0) + 8;
    name_buf = (char *)cJSON_malloc(name_len);
    memset(name_buf, 0, name_len);
    strcpy(name_buf, "copied_");
    strcat(name_buf, got_str ? got_str : "nil");
    cJSON_AddStringToObject(root, name_buf, got_str ? got_str : "");
    replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(arr, s0, replacement);
    cmp_result = cJSON_Compare(replacement, cJSON_GetArrayItem(arr, 0), 1);
    (void)cmp_result;

    // step 4: Cleanup
    cJSON_free(name_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}