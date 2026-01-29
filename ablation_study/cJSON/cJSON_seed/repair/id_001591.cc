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
//<ID> 1591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *base = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "base", base);
    cJSON *shared = cJSON_CreateString("shared_payload");

    // step 2: Configure
    cJSON *base_ref = cJSON_GetObjectItemCaseSensitive(config, "base");
    double base_val = cJSON_GetNumberValue(base_ref);
    double total_val = base_val + 8.5;
    cJSON_AddNumberToObject(root, "total", total_val);
    cJSON_AddItemReferenceToObject(root, "shared_ref", shared);
    cJSON *factor = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(config, "factor", factor);

    // step 3: Operate and Validate
    cJSON *total_ref = cJSON_GetObjectItemCaseSensitive(root, "total");
    double observed_total = cJSON_GetNumberValue(total_ref);
    cJSON *name_ref = cJSON_GetObjectItemCaseSensitive(config, "name");
    const char *name_str = cJSON_GetStringValue(name_ref);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + (int)observed_total % 10);
    if (out_len) {
        scratch[2] = out[0];
    } else {
        scratch[2] = '-';
    }

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}