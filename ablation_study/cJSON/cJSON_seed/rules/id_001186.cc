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
//<ID> 1186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *info = NULL;
    cJSON *enabled = NULL;
    cJSON *value_item = NULL;
    cJSON *root = NULL;
    cJSON *active_item = NULL;
    cJSON *replacement_flag = NULL;
    cJSON *temp = NULL;
    char json[] = "{\"info\":{\"name\":\"test\",\"enabled\":false},\"value\":123}";
    double value = 0.0;
    cJSON_bool is_false = 0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    info = cJSON_GetObjectItem(parsed, "info");
    enabled = cJSON_GetObjectItem(info, "enabled");
    value_item = cJSON_GetObjectItem(parsed, "value");
    value = cJSON_GetNumberValue(value_item);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "value_copy", cJSON_CreateNumber(value));
    cJSON_AddItemToObject(root, "source_enabled", cJSON_CreateBool(cJSON_IsTrue(enabled)));
    cJSON_AddTrueToObject(root, "active");
    active_item = cJSON_GetObjectItem(root, "active");
    replacement_flag = cJSON_CreateFalse();
    temp = cJSON_CreateString("temporary");

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, active_item, replacement_flag);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "active"));
    (void)is_false;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(temp);
    // API sequence test completed successfully
    return 66;
}