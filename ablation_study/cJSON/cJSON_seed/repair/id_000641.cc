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
//<ID> 641
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "example");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *content_item = cJSON_AddStringToObject(payload, "content", "hello world");

    // step 2: Configure
    cJSON *dup_payload = cJSON_Duplicate(payload, 1);
    cJSON_AddItemToObject(root, "payload_copy", dup_payload);
    cJSON *note = cJSON_CreateString("derived-note");
    cJSON_AddItemToObject(dup_payload, "note", note);

    // step 3: Operate and Validate
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_str);
    cJSON_bool was_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "active"));
    cJSON *was_false_item = cJSON_CreateBool(was_false);
    cJSON_AddItemToObject(root, "active_was_false", was_false_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}