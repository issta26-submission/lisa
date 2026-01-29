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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"id\":42,\"name\":\"device\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");

    // step 2: Configure
    cJSON *marker = cJSON_AddNullToObject(root, "marker");
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);

    // step 3: Operate
    double id_val = cJSON_GetNumberValue(id_item);
    double pi_val = cJSON_GetNumberValue(pi_item);
    double total = id_val + pi_val;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *fetched_sum = cJSON_GetObjectItem(root, "sum");
    double fetched_total = cJSON_GetNumberValue(fetched_sum);
    (void)name_item;
    (void)marker;
    (void)fetched_total;

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}