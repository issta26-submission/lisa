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
//<ID> 1092
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 3.14);
    const char json[] = "{\"name\":\"node\",\"value\":10}";
    size_t len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, len);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON *label = cJSON_CreateString("old_label");
    cJSON_AddItemToObject(root, "label", label);
    char *updated_label = cJSON_SetValuestring(label, "new_label");

    // step 3: Operate and Validate
    cJSON *got_parsed = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    cJSON_AddNumberToObject(got_parsed, "updated", 7.0);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}