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
//<ID> 729
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"status\":\"ok\",\"count\":3}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    const char *status_str = cJSON_GetStringValue(status_item);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_SetNumberHelper(count_item, count_val + 1.0);
    cJSON *enabled_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled_flag);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}