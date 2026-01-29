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
//<ID> 999
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\":{\"tags\":[\"alpha\",\"beta\"]},\"name\":\"sample\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *tags = cJSON_GetObjectItem(meta, "tags");
    const char *new_tags[] = { "gamma", "delta" };
    cJSON *new_tags_arr = cJSON_CreateStringArray(new_tags, 2);
    cJSON_ReplaceItemInObject(meta, "tags", new_tags_arr);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "extracted_name", name_val);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    (void)has_meta;

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(1024);
    memset(print_buf, 0, 1024);
    cJSON_PrintPreallocated(root, print_buf, 1024, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "extracted_name");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}