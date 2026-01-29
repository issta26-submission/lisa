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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "name", str_item);

    // step 2: Configure
    cJSON *nested = cJSON_CreateObject();
    cJSON *meta_str = cJSON_CreateString("metainfo");
    cJSON_AddItemToObject(root, "meta", nested);
    cJSON_AddItemToObject(nested, "info", meta_str);
    cJSON *replacement_num = cJSON_CreateNumber(654.321);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "value", replacement_num);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);

    // step 3: Operate and Validate
    cJSON *got_value = cJSON_GetObjectItemCaseSensitive(root, "value");
    double extracted_value = cJSON_GetNumberValue(got_value);
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(got_name);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    int summary = (int)extracted_value + (int)replaced + (int)name_is_string + (dup_root ? dup_root->type : 0) + (root ? root->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(tmp_buf);
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}