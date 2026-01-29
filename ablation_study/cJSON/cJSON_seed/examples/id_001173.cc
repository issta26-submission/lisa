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
//<ID> 1173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":true,\"beta\":null,\"num\":4.25}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"inner\":123}");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "rawdata", raw_item);
    cJSON_AddItemToObjectCS(root, "always_true", true_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(parsed, "num");
    double num_value = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "num_copy", num_value);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}