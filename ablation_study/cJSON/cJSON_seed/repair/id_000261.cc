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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"a\":1.0,\"b\":2.5}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "data", parsed);
    cJSON *replacement = cJSON_CreateNumber(100.75);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(parsed, "a", replacement);

    // step 3: Operate and Validate
    cJSON *a_item = cJSON_GetObjectItemCaseSensitive(parsed, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON_AddItemToObject(root, "a_value", cJSON_CreateNumber(a_val));
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)replaced;
    (void)parse_end;
    (void)a_val;
    return 66;
}