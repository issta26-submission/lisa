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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    char *tmp = (char *)cJSON_malloc(64);
    memset(tmp, 0, 64);
    int nums_size = 0;
    cJSON *found_metrics = NULL;
    cJSON *found_label = NULL;
    const char *label_text = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObject(metrics, "numbers", nums);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddNumberToObject(root, "threshold", 5.0);
    cJSON_AddStringToObject(metrics, "label", "sensor_A");

    // step 3: Operate / Validate
    nums_size = cJSON_GetArraySize(nums);
    found_metrics = cJSON_GetObjectItem(root, "metrics");
    found_label = cJSON_GetObjectItem(found_metrics, "label");
    label_text = cJSON_GetStringValue(found_label);
    tmp[0] = (char)('0' + (nums_size % 10));
    (void)label_text;

    // step 4: Cleanup
    cJSON_free(tmp);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}