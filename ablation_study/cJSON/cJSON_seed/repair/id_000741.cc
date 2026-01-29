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
//<ID> 741
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 10.5);
    cJSON *limit = cJSON_AddNumberToObject(config, "limit", 100.0);
    cJSON *ver = cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddNumberToObject(values, "placeholder_index_0", 0.0); // small demonstration item in array as object child

    // step 3: Operate and Validate
    double thr_val = cJSON_GetNumberValue(threshold);
    cJSON *detached_limit = cJSON_DetachItemFromObject(config, "limit");
    double detached_val = cJSON_GetNumberValue(detached_limit);
    cJSON_AddItemToObject(root, "detached_limit", detached_limit);
    double sum = thr_val + detached_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_bool is_obj = cJSON_IsObject(config);
    cJSON_AddNumberToObject(root, "is_object_flag", (double)is_obj);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}