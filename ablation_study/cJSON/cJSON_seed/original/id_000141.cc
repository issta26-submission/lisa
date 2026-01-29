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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "payload", payload);
    cJSON_AddItemToObjectCS(payload, "value", cJSON_CreateNumber(123.0));
    cJSON_AddItemToObjectCS(payload, "empty", cJSON_CreateNull());
    cJSON_AddItemToObjectCS(payload, "text", cJSON_CreateString("example"));

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON *dup_payload = cJSON_Duplicate(payload, 1);
    cJSON_AddItemToObjectCS(root, "payload_copy", dup_payload);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *nil_item = cJSON_GetObjectItemCaseSensitive(payload, "empty");
    cJSON_bool is_nil = cJSON_IsNull(nil_item);
    int summary = (printed ? (int)printed[0] : 0) + (int)is_nil + root->type + payload->type + dup_payload->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}