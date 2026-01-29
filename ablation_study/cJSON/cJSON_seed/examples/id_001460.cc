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
//<ID> 1460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"num\": 42, \"nested\": {\"key\":\"value\"}}";
    cJSON *root = cJSON_Parse(json_text);
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);

    // step 2: Configure
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON_bool num_is_number = cJSON_IsNumber(num_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)num_is_number;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}