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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"alpha\",\"value\":42}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double orig_value = cJSON_GetNumberValue(value_item);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(orig_value + 3.1415);
    cJSON_AddItemToObject(root, "value_plus_pi", new_num);
    cJSON *marker = cJSON_AddNullToObject(root, "marker");

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItem(root, "value_plus_pi");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    (void)retrieved_val;
    (void)marker;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}