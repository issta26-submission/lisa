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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\": 123, \"b\": {\"inner\": \"value\"}}";
    const char *parse_end = 0;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *obj_b = cJSON_GetObjectItem(root, "b");
    cJSON_bool added_ref = cJSON_AddItemReferenceToObject(root, "b_ref", obj_b);
    (void)added_ref;
    cJSON_bool has_a_before = cJSON_HasObjectItem(root, "a");
    (void)has_a_before;

    // step 3: Operate
    cJSON *target_a = cJSON_GetObjectItem(root, "a");
    cJSON *replacement_false = cJSON_CreateFalse();
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, target_a, replacement_false);
    (void)replaced;
    cJSON_bool has_a_after = cJSON_HasObjectItem(root, "a");
    (void)has_a_after;

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    if (printed_len > 0) {
        memcpy(buf, printed, printed_len + 1);
    }
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}