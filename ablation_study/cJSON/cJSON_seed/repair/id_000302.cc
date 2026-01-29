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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.5);

    // step 2: Operate and Configure
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *version_item = cJSON_GetObjectItem(meta_item, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    double sum = count_val + version_val;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Serialize
    char *formatted = cJSON_Print(root);
    char *unformatted_meta = cJSON_PrintUnformatted(meta_item);

    // step 4: Validate and Cleanup
    cJSON *double_sum = cJSON_CreateNumber(sum * 2.0);
    cJSON_AddItemToObject(root, "double_sum", double_sum);
    char *final_unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(formatted);
    cJSON_free(unformatted_meta);
    cJSON_free(final_unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}