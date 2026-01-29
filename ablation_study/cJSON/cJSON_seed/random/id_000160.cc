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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *metrics = (cJSON *)0;
    cJSON *count_num = (cJSON *)0;
    cJSON *ratio_num = (cJSON *)0;
    cJSON *got_metrics = (cJSON *)0;
    cJSON *metrics_is_object_num = (cJSON *)0;
    char *json_text = (char *)0;
    int count = 2;
    double ratio = 3.14159;
    cJSON_bool is_obj = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    metrics = cJSON_CreateObject();
    count_num = cJSON_CreateNumber((double)count);
    ratio_num = cJSON_CreateNumber(ratio);

    // step 3: Configure
    cJSON_AddItemToObject(metrics, "count", count_num);
    cJSON_AddItemToObject(metrics, "ratio", ratio_num);
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 4: Operate
    got_metrics = cJSON_GetObjectItem(root, "metrics");
    is_obj = cJSON_IsObject(got_metrics);
    metrics_is_object_num = cJSON_CreateNumber((double)is_obj);
    cJSON_AddItemToObject(root, "metrics_is_object", metrics_is_object_num);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}