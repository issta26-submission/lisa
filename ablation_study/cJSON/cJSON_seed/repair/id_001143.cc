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
//<ID> 1143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *vals = cJSON_AddArrayToObject(root, "values");
    cJSON_AddNumberToObject(cfg, "threshold", 2.5);
    cJSON *v1 = cJSON_CreateNumber(10.0);
    cJSON *v2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(vals, v1);
    cJSON_AddItemToArray(vals, v2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "note", "sample");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *cfg_ref = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(cfg_ref, "threshold");
    double threshold = cJSON_GetNumberValue(threshold_item);
    cJSON *values_ref = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *first = cJSON_GetArrayItem(values_ref, 0);
    double firstv = cJSON_GetNumberValue(first);
    double computed = firstv * threshold;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}