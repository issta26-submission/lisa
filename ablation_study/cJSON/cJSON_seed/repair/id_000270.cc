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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\":2.5,\"b\":true}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *a_item = cJSON_GetObjectItem(parsed, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON_AddNumberToObject(root, "copied_a", a_val);
    cJSON_AddItemToObject(root, "original", parsed);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());

    // step 3: Operate and Validate
    cJSON *verify_item = cJSON_GetObjectItem(root, "copied_a");
    double verify_val = cJSON_GetNumberValue(verify_item);
    cJSON_AddNumberToObject(root, "checked_a", verify_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}