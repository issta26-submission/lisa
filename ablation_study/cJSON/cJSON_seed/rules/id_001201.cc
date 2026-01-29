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
//<ID> 1201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *ref = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON *nested = NULL;
    cJSON *nested_val = NULL;
    char *printed = NULL;
    const char json[] = "{\"key\": false, \"nested\": {\"val\": 123.5}}";
    size_t json_len = sizeof(json) - 1;
    cJSON_bool is_key_false = 0;
    double nested_number = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, json_len);
    true_item = cJSON_CreateTrue();
    root = cJSON_CreateObject();
    ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObject(root, "remote", ref);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    nested = cJSON_GetObjectItem(parsed, "nested");
    nested_val = cJSON_GetObjectItem(nested, "val");
    nested_number = cJSON_GetNumberValue(nested_val);
    is_key_false = cJSON_IsFalse(cJSON_GetObjectItem(parsed, "key"));
    printed = cJSON_PrintUnformatted(root);
    (void)retrieved_flag;
    (void)nested_number;
    (void)is_key_false;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}