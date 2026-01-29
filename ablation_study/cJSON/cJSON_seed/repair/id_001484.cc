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
//<ID> 1484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"metric\":42.5,\"arr\":[10,20,30]}";
    size_t len = (size_t)strlen(json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);

    // step 2: Configure
    cJSON *metric_item = cJSON_GetObjectItemCaseSensitive(root, "metric");
    double metric = cJSON_GetNumberValue(metric_item);
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "metric_copy", metric);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *summary_obj = cJSON_GetObjectItemCaseSensitive(root, "summary");
    cJSON *metric_copy_item = cJSON_GetObjectItemCaseSensitive(summary_obj, "metric_copy");
    double metric_copy_value = cJSON_GetNumberValue(metric_copy_item);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = (char)('0' + ((int)metric_copy_value % 10));
    scratch[1] = (char)('0' + ((int)first_value % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}