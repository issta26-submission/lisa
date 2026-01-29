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
//<ID> 1200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *true_node = NULL;
    cJSON *ref = NULL;
    char *printed = NULL;
    double value = 0.0;
    cJSON_bool is_false = 0;
    const char json[] = "{\"flag\":false,\"value\":123.5}";
    size_t len = sizeof(json) - 1;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    flag = cJSON_GetObjectItem(parsed, "flag");
    is_false = cJSON_IsFalse(flag);
    true_node = cJSON_CreateTrue();
    ref = cJSON_CreateObjectReference(parsed);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data_ref", ref);
    cJSON_AddItemToObject(root, "override_flag", true_node);

    // step 3: Operate / Validate
    value = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "value"));
    printed = cJSON_PrintUnformatted(root);
    (void)is_false;
    (void)value;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}