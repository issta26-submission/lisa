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
//<ID> 788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "{\"alpha\":10, \"beta\":\"two\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "new_number", 42.5);
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddNumberToObject(parsed, "added_to_parsed", 7.0);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "beta");
    cJSON_AddItemToObject(root, "detached_beta", detached);
    double alpha_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "alpha"));
    cJSON_AddNumberToObject(root, "alpha_copy", alpha_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}