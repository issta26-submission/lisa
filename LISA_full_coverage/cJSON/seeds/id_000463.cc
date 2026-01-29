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
//<ID> 463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *marker = cJSON_AddNullToObject(root, "marker");

    // step 2: Configure
    cJSON *extra_item = cJSON_CreateNumber(58.0);
    cJSON_AddItemToObject(root, "extra", extra_item);
    cJSON *retrieved_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(retrieved_count);
    cJSON *retrieved_extra = cJSON_GetObjectItem(root, "extra");
    double extra_val = cJSON_GetNumberValue(retrieved_extra);

    // step 3: Operate
    double total = count_val + extra_val;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *got_marker = cJSON_GetObjectItem(root, "marker");
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)got_marker;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}