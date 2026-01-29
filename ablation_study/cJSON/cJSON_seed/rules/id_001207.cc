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
//<ID> 1207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *status = NULL;
    cJSON *true_item = NULL;
    cJSON *ref = NULL;
    char *printed = NULL;
    double value = 0.0;
    cJSON_bool status_is_false = 0;
    const char json[] = "{\"status\":false,\"value\":123}";
    size_t json_len = sizeof(json) - 1;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLength(json, json_len);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "new_true", true_item);
    ref = cJSON_Duplicate(true_item, 0); // create a separate item instead of a reference
    cJSON_AddItemToObject(root, "alias_true", ref);

    // step 3: Operate / Validate
    status = cJSON_GetObjectItem(root, "status");
    status_is_false = cJSON_IsFalse(status);
    value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    printed = cJSON_PrintUnformatted(root);
    (void)status_is_false;
    (void)value;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}