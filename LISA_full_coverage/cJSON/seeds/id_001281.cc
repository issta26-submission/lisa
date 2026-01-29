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
//<ID> 1281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"flag\":true,\"value\":123,\"nested\":{\"a\":1}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON_bool has_value = cJSON_HasObjectItem(root, "value");
    cJSON *has_value_node = cJSON_CreateBool(has_value);
    cJSON_AddItemToObject(root, "has_value", has_value_node);
    cJSON *dup_nested = cJSON_Duplicate(nested, 1);
    cJSON_AddItemReferenceToObject(root, "nested_ref", dup_nested);
    cJSON *false_node = cJSON_CreateFalse();
    cJSON_AddItemReferenceToObject(root, "false_ref", false_node);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(999.0);
    cJSON_ReplaceItemViaPointer(root, value_item, replacement);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}