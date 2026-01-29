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
//<ID> 1179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":true,\"beta\":null,\"count\":2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *explicit_true = cJSON_CreateTrue();
    cJSON *raw_item = cJSON_CreateRaw("embedded:raw");

    // step 2: Configure
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddItemToObjectCS(root, "explicitTrue", explicit_true);
    cJSON_AddItemToObjectCS(root, "raw", raw_item);

    // step 3: Operate & Validate
    cJSON *alpha = cJSON_GetObjectItem(parsed, "alpha");
    cJSON_bool alpha_true = cJSON_IsTrue(alpha);
    cJSON_AddNumberToObject(root, "alpha_as_number", (double)alpha_true);
    cJSON *count = cJSON_GetObjectItem(parsed, "count");
    double count_val = cJSON_GetNumberValue(count);
    cJSON_AddNumberToObject(root, "count_copy", count_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}