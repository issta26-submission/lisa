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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", num_item);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", str_item);

    // step 2: Configure
    char *json_unfmt = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_unfmt);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = 'x';
    scratch[1] = '\0';
    cJSON *marker = cJSON_CreateString(scratch);
    cJSON_AddItemToObject(root, "marker", marker);

    // step 3: Operate and Validate
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON_bool is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "nullable"));
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup_root, 1);
    int summary = (int)count_val + (int)(cJSON_IsString(str_item)) + (int)is_null + (int)equal + (root ? root->type : 0);
    (void)name_val;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(json_unfmt);
    cJSON_free(scratch);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}