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
//<ID> 1140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_item = NULL;
    cJSON *replacement = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *first_item = NULL;
    cJSON *copied_first = NULL;
    char *out_root = NULL;
    char *out_parsed = NULL;
    char *dyn = NULL;
    const char json[] = "{\"name\":\"example\",\"values\":[\"v1\",\"v2\"]}";
    size_t dyn_size = 16;
    cJSON_bool isstr;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_item = cJSON_CreateString("placeholder");
    cJSON_AddItemToObject(root, "data", old_item);
    replacement = cJSON_CreateArray();
    cJSON_AddItemToArray(replacement, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(replacement, cJSON_CreateNumber(2.0));
    dyn = (char *)cJSON_malloc(dyn_size);
    memset(dyn, 0, dyn_size);
    dyn[0] = 'd';
    dyn[1] = 'y';
    dyn[2] = 'n';
    dyn[3] = '1';
    cJSON_AddItemToArray(replacement, cJSON_CreateString(dyn));

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_item, replacement);
    parsed = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(parsed, "values");
    first_item = cJSON_GetArrayItem(arr, 0);
    isstr = cJSON_IsString(first_item);
    (void)isstr;
    copied_first = cJSON_CreateString(cJSON_GetStringValue(first_item));
    cJSON_AddItemToObject(root, "first_parsed", copied_first);
    out_root = cJSON_Print(root);
    out_parsed = cJSON_Print(parsed);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(out_root);
    cJSON_free(out_parsed);
    cJSON_free(dyn);
    // API sequence test completed successfully
    return 66;
}