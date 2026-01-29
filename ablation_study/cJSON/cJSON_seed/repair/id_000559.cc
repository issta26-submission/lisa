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
//<ID> 559
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(stats, "value", num_item);
    cJSON_AddNumberToObject(root, "extra", 7.0);

    // step 2: Configure
    double newval = cJSON_SetNumberHelper(num_item, 99.75);
    cJSON *derived = cJSON_CreateNumber(newval * 2.0);
    cJSON_AddItemToObject(stats, "double_value", derived);
    cJSON *stats_copy = cJSON_Duplicate(stats, 1);
    cJSON_AddItemToObject(root, "stats_copy", stats_copy);

    // step 3: Operate and Validate
    cJSON *copied_value_item = cJSON_GetObjectItem(stats_copy, "value");
    double fetched = cJSON_GetNumberValue(copied_value_item);
    cJSON_AddNumberToObject(root, "fetched_sum", fetched + newval);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}