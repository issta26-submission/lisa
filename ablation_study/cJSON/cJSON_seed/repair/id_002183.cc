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
//<ID> 2183
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
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(payload, "values", values);

    // step 2: Configure
    cJSON *pi = cJSON_AddNumberToObject(payload, "pi", 3.14159265);
    cJSON *to_replace = cJSON_AddNumberToObject(payload, "replace_me", 1.0);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);

    // step 3: Operate and Validate
    cJSON_bool pi_is_number = cJSON_IsNumber(pi);
    cJSON *pi_marker = cJSON_CreateBool(pi_is_number);
    cJSON_AddItemToObject(payload, "pi_is_number", pi_marker);
    cJSON *replacement = cJSON_CreateNumber(99.99);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(payload, "replace_me", replacement);
    cJSON *replaced_marker = cJSON_CreateBool(replaced);
    cJSON_AddItemToObject(payload, "replace_succeeded", replaced_marker);
    cJSON_DeleteItemFromObjectCaseSensitive(payload, "pi");
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}