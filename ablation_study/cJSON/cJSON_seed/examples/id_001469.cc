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
//<ID> 1469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    const char *json_text = "{\"id\":42,\"name\":\"test\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *ver_str = cJSON_CreateString(version);

    // step 2: Configure
    cJSON *added_str = cJSON_CreateString("added");
    cJSON_AddItemToObject(root, "added", added_str);
    cJSON_AddItemToObject(root, "version", ver_str);

    // step 3: Operate & Validate
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    cJSON_bool id_is_number = cJSON_IsNumber(id_item);
    double id_value = cJSON_GetNumberValue(id_item);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_value = cJSON_GetStringValue(name_item);
    char *out = cJSON_PrintUnformatted(root);
    (void)id_is_number;
    (void)id_value;
    (void)name_value;
    (void)out;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}