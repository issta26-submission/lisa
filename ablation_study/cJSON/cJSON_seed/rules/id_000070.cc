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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_item = NULL;
    cJSON *value_item = NULL;
    cJSON *name_ref = NULL;
    char *printed = NULL;
    char *name = NULL;
    double val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));
    cJSON_AddNumberToObject(root, "value", 42.0);
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    value_item = cJSON_GetObjectItem(root, "value");
    val = cJSON_GetNumberValue(value_item);
    name_item = cJSON_GetObjectItem(root, "name");
    name = cJSON_GetStringValue(name_item);
    name_ref = cJSON_CreateStringReference(name);
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    cJSON_AddNumberToObject(root, "double_value", val * 2.0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}