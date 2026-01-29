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
//<ID> 979
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char original_json[] = "{ \"config\": { \"name\": \"test\" }, \"values\": [1, 2, 3] }";
    size_t original_len = sizeof(original_json) - 1;
    char *mutable_json = (char *)cJSON_malloc(original_len + 1);
    memcpy(mutable_json, original_json, original_len + 1);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(values, null_item);
    int values_count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "values_count", (double)values_count);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_free(mutable_json);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}