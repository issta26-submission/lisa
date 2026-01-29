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
//<ID> 807
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON *str_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToObject(root, "active", true_item);
    cJSON_AddItemToObject(root, "count", num_item);
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddRawToObject(root, "raw", "{\"embedded\":1}");
    cJSON_AddStringToObject(meta, "note", "nested");

    // step 3: Operate and Validate
    cJSON *active = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON_bool is_active = cJSON_IsTrue(active);
    cJSON *count = cJSON_GetObjectItemCaseSensitive(root, "count");
    double count_val = cJSON_GetNumberValue(count);
    double score = count_val + (double)is_active;
    cJSON_AddNumberToObject(meta, "score", score);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}