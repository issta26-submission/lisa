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
//<ID> 1006
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *active_item = NULL;
    cJSON *optional_item = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    const char *device_name = "sensor_X";
    const char *got_name = NULL;
    cJSON_bool active_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString(device_name);
    cJSON_AddItemToObject(root, "name", name_item);
    active_item = cJSON_AddTrueToObject(root, "active");
    optional_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "name");
    got_name = cJSON_GetStringValue(retrieved);
    cJSON_AddItemToObject(root, "name_copy", cJSON_CreateString(got_name));
    active_flag = cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}