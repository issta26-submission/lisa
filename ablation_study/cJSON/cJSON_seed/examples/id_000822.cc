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
//<ID> 822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *nil = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nil", nil);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("\"rawvalue\"");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "count", count);

    // step 3: Operate & Validate
    cJSON_bool nil_is_null = cJSON_IsNull(nil);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *pi_copy = cJSON_CreateNumber(pi_val);
    cJSON_AddItemToObject(root, "pi_copy", pi_copy);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_pi_copy = cJSON_GetObjectItem(parsed, "pi_copy");
    double parsed_val = cJSON_GetNumberValue(parsed_pi_copy);
    (void)nil_is_null;
    (void)raw_is_raw;
    (void)parsed_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}