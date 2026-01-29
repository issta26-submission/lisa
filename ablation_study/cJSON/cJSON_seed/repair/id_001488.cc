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
//<ID> 1488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"values\":[10,20,30],\"meta\":{\"num\":77}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)strlen(json) + 1, &parse_end, 1);
    cJSON *values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");

    // step 2: Configure
    cJSON *collector = cJSON_CreateObject();
    cJSON *meta_num_item = cJSON_GetObjectItemCaseSensitive(meta, "num");
    double meta_num = cJSON_GetNumberValue(meta_num_item);
    cJSON_AddNumberToObject(collector, "meta_copy", meta_num);
    cJSON_AddItemToObject(root, "collector", collector);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    cJSON_DeleteItemFromArray(values, 1);
    cJSON *post_item = cJSON_GetArrayItem(values, 1);
    double post_val = cJSON_GetNumberValue(post_item);
    cJSON_AddNumberToObject(collector, "post_deletion", post_val);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}