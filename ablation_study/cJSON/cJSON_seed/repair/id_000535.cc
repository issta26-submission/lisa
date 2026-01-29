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
//<ID> 535
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *deprecated = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "deprecated", deprecated);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *submeta = cJSON_AddObjectToObject(meta, "submeta");
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(submeta, "pi", pi);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    const cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    const cJSON *pi_item = cJSON_GetObjectItem(submeta, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    double total = count_val + pi_val;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(dup, "total", total_item);

    // step 3: Operate
    cJSON_AddItemToObject(dup, "copied_from_original", cJSON_CreateTrue());
    char *out = cJSON_PrintUnformatted(dup);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}