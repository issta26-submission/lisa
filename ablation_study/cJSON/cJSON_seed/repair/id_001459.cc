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
//<ID> 1459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "cfg");
    cJSON *items = cJSON_AddArrayToObject(cfg, "items");

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example-name");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(123.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("second"));
    cJSON_AddItemToArray(items, cJSON_CreateTrue());
    cJSON_AddNumberToObject(root, "version", 2.5);

    // step 3: Operate and Validate
    cJSON *cfg_item = cJSON_GetObjectItemCaseSensitive(root, "cfg");
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "cfg");
    cJSON *items_item = cJSON_GetObjectItemCaseSensitive(cfg_item, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items_item);
    int count = cJSON_GetArraySize(items_item);
    cJSON *second_item = cJSON_GetArrayItem(items_item, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)(items_is_array ? '1' : '0');
    scratch[3] = second_str ? second_str[0] : 'n';
    scratch[4] = (char)(has_cfg ? 'Y' : 'N');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}