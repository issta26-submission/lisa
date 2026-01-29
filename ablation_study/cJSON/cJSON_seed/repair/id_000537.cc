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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *metadata = cJSON_AddObjectToObject(root, "metadata");
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(metadata, "active", active);
    cJSON *meta_copy = cJSON_Duplicate(metadata, 1);
    cJSON_AddItemToObject(root, "metadata_copy", meta_copy);

    // step 3: Operate and Validate
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON *increment = cJSON_CreateNumber(count_val + 1.0);
    cJSON_AddItemToObject(metadata, "count_plus_one", increment);
    cJSON *retrieved = cJSON_GetObjectItem(metadata, "count_plus_one");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    cJSON *sum_item = cJSON_CreateNumber(retrieved_val + count_val);
    cJSON_AddItemToObject(root, "sum_counts", sum_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}