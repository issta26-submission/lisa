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
//<ID> 1453
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
    cJSON_AddStringToObject(cfg, "name", "fuzz-test");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON *s_item = cJSON_CreateString("str-item");
    cJSON_AddItemToArray(items, s_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "api-sequence-test");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *items_case = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "cfg");
    cJSON_bool items_are_array = cJSON_IsArray(items_case);
    int items_count = cJSON_GetArraySize(items_case);
    cJSON *first_item = cJSON_GetArrayItem(items_case, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len > 0 ? out[0] : ' ';
    scratch[1] = (char)('0' + (items_count % 10));
    scratch[2] = (char)(items_are_array ? '1' : '0');
    scratch[3] = (char)(has_cfg ? '1' : '0');
    scratch[4] = (char)('0' + ((int)first_val % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}