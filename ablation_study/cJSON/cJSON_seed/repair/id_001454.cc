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
//<ID> 1454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *manual_str = cJSON_CreateString("manual-string");

    // step 2: Configure
    cJSON_AddItemToObject(root, "manual_str", manual_str);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("elem-two"));
    cJSON_AddNumberToObject(root, "version", 2.5);
    cJSON_AddFalseToObject(meta, "enabled");

    // step 3: Operate and Validate
    cJSON *items_item = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON_bool items_are_array = cJSON_IsArray(items_item);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *manual_item = cJSON_GetObjectItemCaseSensitive(root, "manual_str");
    const char *manual_value = cJSON_GetStringValue(manual_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len > 0 ? out[0] : ' ';
    scratch[1] = (char)(items_are_array ? '1' : '0');
    scratch[2] = (char)(has_meta ? '1' : '0');
    scratch[3] = manual_value ? manual_value[0] : 'n';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}