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
//<ID> 1127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    cJSON_AddNumberToObject(stats, "mean", 42.5);
    cJSON_AddNumberToObject(stats, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *stats_ref = cJSON_GetObjectItemCaseSensitive(root, "stats");
    cJSON *mean_item = cJSON_GetObjectItemCaseSensitive(stats_ref, "mean");
    double mean_val = cJSON_GetNumberValue(mean_item);
    cJSON *first_val = cJSON_GetArrayItem(arr, 0);
    double first_num = cJSON_GetNumberValue(first_val);
    double computed = mean_val + first_num;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}