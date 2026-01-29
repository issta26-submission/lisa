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
//<ID> 1282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    const char *json = "{\"parsed\": {\"val\": 123}}";
    const char *endptr = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &endptr, 1);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "parsed");
    cJSON_AddItemReferenceToObject(root, "parsed_ref", parsed_child);
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "parsed");
    (void)detached;
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "parsed_ref");
    (void)has_ref;

    // step 3: Operate
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *orig_child = cJSON_GetObjectItem(root, "child");
    cJSON_ReplaceItemViaPointer(root, orig_child, false_item);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}