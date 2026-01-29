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
//<ID> 1539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *active_item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active_item);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw", "{\"inner\":42}");

    // step 2: Configure
    char *printed = cJSON_Print(root);
    cJSON *active_case = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON *raw_case = cJSON_GetObjectItemCaseSensitive(root, "raw");

    // step 3: Operate & Validate
    cJSON_bool active_is_bool = cJSON_IsBool(active_case);
    const char *name_value = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(root, "name"));
    (void)active_is_bool; (void)name_value; (void)raw_case; (void)raw_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}