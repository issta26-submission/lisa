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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"example\",\"count\":5}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double orig_count = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    double adjusted_value = orig_count + 2.5;
    cJSON *adjusted_num = cJSON_CreateNumber(adjusted_value);
    cJSON_AddItemToObject(root, "adjusted", adjusted_num);
    cJSON *marker = cJSON_AddNullToObject(root, "marker");

    // step 3: Operate
    cJSON *fetched_adjusted = cJSON_GetObjectItem(root, "adjusted");
    double fetched_value = cJSON_GetNumberValue(fetched_adjusted);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)fetched_value;
    (void)marker;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}