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
//<ID> 825
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta_raw = NULL;
    cJSON *flag_false = NULL;
    cJSON *name_str = NULL;
    cJSON *num_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *orig_meta = NULL;
    cJSON *dup_meta = NULL;
    cJSON *picked_name_item = NULL;
    cJSON *picked_num_item = NULL;
    char *printed = NULL;
    double num_value = 0.0;
    cJSON_bool metas_equal = 0;
    cJSON_bool name_is_string = 0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta_raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", meta_raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    name_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_str);
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    orig_meta = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    dup_meta = cJSON_GetObjectItemCaseSensitive(dup_root, "meta_raw");
    metas_equal = cJSON_Compare(orig_meta, dup_meta, 1);
    picked_num_item = cJSON_GetObjectItem(root, "value");
    num_value = cJSON_GetNumberValue(picked_num_item);
    picked_name_item = cJSON_GetObjectItem(root, "name");
    name_is_string = cJSON_IsString(picked_name_item);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)num_value;
    (void)metas_equal;
    (void)name_is_string;
    return 66;
}