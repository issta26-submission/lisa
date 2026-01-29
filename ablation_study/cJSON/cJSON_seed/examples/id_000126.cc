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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *samples = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "samples", samples);

    // step 2: Configure
    cJSON *count = cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON *avg = cJSON_AddNumberToObject(metrics, "avg", 3.14);
    cJSON *s1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(samples, s1);
    cJSON *s2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(samples, s2);

    // step 3: Operate & Validate
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON *sample0 = cJSON_GetArrayItem(samples, 0);
    double sample0_val = cJSON_GetNumberValue(sample0);
    cJSON *sample1 = cJSON_GetArrayItem(samples, 1);
    double sample1_val = cJSON_GetNumberValue(sample1);
    double total_val = count_val + sample0_val + sample1_val;
    cJSON *total_item = cJSON_CreateNumber(total_val);
    cJSON_AddItemToObject(root, "total", total_item);
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}