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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"title\":\"example\",\"count\":10}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *incr = cJSON_CreateNumber(count_value + 2.5);
    cJSON_AddItemToObject(root, "incremented", incr);
    cJSON *marker = cJSON_AddNullToObject(root, "marker");

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItem(root, "incremented");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)retrieved_val;
    (void)count_value;
    (void)marker;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}