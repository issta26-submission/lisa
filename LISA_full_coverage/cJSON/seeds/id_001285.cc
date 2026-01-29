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
//<ID> 1285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\": 123, \"b\": true}";
    const char *endptr = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &endptr, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *item_a = cJSON_GetObjectItem(parsed, "a");
    cJSON_AddItemReferenceToObject(root, "a_ref", item_a);
    cJSON *replacement = cJSON_CreateFalse();

    // step 3: Operate
    cJSON *item_b = cJSON_GetObjectItem(parsed, "b");
    cJSON_ReplaceItemViaPointer(parsed, item_b, replacement);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "a_ref");
    (void)has_ref;
    char *printed = cJSON_PrintUnformatted(parsed);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_DetachItemViaPointer(root, item_a);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}