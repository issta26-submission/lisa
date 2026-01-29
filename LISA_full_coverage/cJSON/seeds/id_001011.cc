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
//<ID> 1011
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"alpha\":null,\"count\":42,\"flag\":false}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *alpha_item = cJSON_GetObjectItem(root, "alpha");
    cJSON_bool alpha_was_null = cJSON_IsNull(alpha_item);
    cJSON_AddBoolToObject(root, "alpha_was_null", alpha_was_null);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "double_count", count_val * 2.0);
    cJSON *meta_obj = cJSON_CreateObject();
    cJSON *note_str = cJSON_CreateString("generated_by_test");
    cJSON_AddItemToObject(meta_obj, "note", note_str);
    cJSON_AddItemToObject(root, "meta", meta_obj);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_AddBoolToObject(root, "has_meta", has_meta);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "flag");
    cJSON_DeleteItemFromObject(root, "alpha");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}