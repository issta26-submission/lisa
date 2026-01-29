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
//<ID> 1456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "title", cJSON_CreateString("api-sequence-test"));
    cJSON_AddNumberToObject(meta, "version", 3.14);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(100.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(200.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("last"));

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddStringToObject(root, "note", "fuzzer-seq");

    // step 3: Operate and Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *items_node = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON_bool items_are_array = cJSON_IsArray(items_node);
    cJSON *meta_node = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *title_node = cJSON_GetObjectItemCaseSensitive(meta_node, "title");
    char *title_str = cJSON_GetStringValue(title_node);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len > 0 ? out[0] : ' ';
    scratch[1] = (char)('0' + (has_meta ? 1 : 0));
    scratch[2] = (char)(items_are_array ? 'A' : 'O');
    scratch[3] = title_str ? title_str[0] : 'n';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}