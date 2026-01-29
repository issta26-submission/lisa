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
//<ID> 1451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *list = cJSON_AddArrayToObject(root, "list");
    cJSON_AddItemToArray(list, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(list, cJSON_CreateFalse());
    cJSON *strnode = cJSON_CreateString("str-item");
    cJSON_AddItemToArray(list, strnode);
    cJSON_AddStringToObject(root, "title", "api-seq");

    // step 2: Configure
    cJSON *cfg_get = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON_bool has_enabled = cJSON_HasObjectItem(cfg_get, "enabled");
    cJSON *title_item = cJSON_GetObjectItemCaseSensitive(root, "title");
    char *title_str = cJSON_GetStringValue(title_item);
    cJSON *list_item = cJSON_GetObjectItemCaseSensitive(root, "list");
    cJSON_bool list_is_array = cJSON_IsArray(list_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len > 0 ? out[0] : ' ';
    scratch[1] = (char)('0' + (has_enabled ? 1 : 0));
    scratch[2] = (char)('0' + (list_is_array ? 1 : 0));
    scratch[3] = title_str ? title_str[0] : 'n';
    scratch[4] = (char)('0' + (cJSON_GetArraySize(list) % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}