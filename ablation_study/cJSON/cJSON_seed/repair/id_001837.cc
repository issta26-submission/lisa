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
//<ID> 1837
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"root\":{\"items\":[10,20,30],\"name\":\"alpha\"}}";
    size_t raw_len = strlen(raw_json);
    char *json_buf = (char *)cJSON_malloc(raw_len + 1);
    memcpy(json_buf, raw_json, raw_len + 1);
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *inner = cJSON_GetObjectItem(root, "root");
    cJSON *items = cJSON_GetObjectItem(inner, "items");
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddItemToObject(meta, "copied", dup_items);
    cJSON *name_item = cJSON_GetObjectItem(inner, "name");
    cJSON_SetValuestring(name_item, "beta");

    // step 3: Operate and Validate
    int original_size = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(dup_items, 1);
    int dup_size = cJSON_GetArraySize(dup_items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    const char *name_str = cJSON_GetStringValue(name_item);
    scratch[0] = printed ? printed[0] : 'X';
    scratch[1] = (char)('0' + (original_size % 10));
    scratch[2] = (char)('0' + (dup_size % 10));
    scratch[3] = name_str ? name_str[0] : 'N';
    (void)first_val; (void)scratch; (void)name_str; (void)printed;

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}