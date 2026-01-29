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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\":42,\"replace_me\":10}";
    size_t len = (sizeof(json) - 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);

    // step 2: Configure
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value = cJSON_GetNumberValue(value_item);
    cJSON *doubled = cJSON_CreateNumber(value * 2.0);
    cJSON_AddItemToObject(root, "doubled", doubled);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateNumber(12345.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", replacement);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}