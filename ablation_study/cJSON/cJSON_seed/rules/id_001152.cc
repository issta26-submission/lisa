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
//<ID> 1152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *beta = NULL;
    cJSON *dup_beta = NULL;
    cJSON *num_item = NULL;
    cJSON *inner = NULL;
    const char *inner_str = NULL;
    const char *parse_end = NULL;
    const char json[] = "{ \"alpha\": 1, \"beta\": { \"inner\":\"value\" } }";
    size_t json_len = (size_t)(sizeof(json) - 1);
    char *mutable_json = (char *)cJSON_malloc(json_len + 1);
    memset(mutable_json, 0, json_len + 1);
    memcpy(mutable_json, json, json_len);

    // step 2: Setup / Configure
    cJSON_Minify(mutable_json);
    root = cJSON_CreateObject();
    cJSON_AddNullToObject(root, "no_value");
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    parsed = cJSON_ParseWithLengthOpts(mutable_json, json_len, &parse_end, 1);
    beta = cJSON_GetObjectItem(parsed, "beta");

    // step 3: Operate / Validate
    dup_beta = cJSON_Duplicate(beta, 1);
    cJSON_AddItemToObject(root, "embedded_beta", dup_beta);
    inner = cJSON_GetObjectItem(beta, "inner");
    inner_str = cJSON_GetStringValue(inner);
    cJSON_AddItemToObject(root, "inner_copy", cJSON_CreateString(inner_str));

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}