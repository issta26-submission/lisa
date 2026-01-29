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
//<ID> 829
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *value_num = NULL;
    cJSON *value_str = NULL;
    cJSON *raw = NULL;
    cJSON *flag_false = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_value = NULL;
    cJSON *dup_name = NULL;
    cJSON *orig_raw = NULL;
    cJSON *dup_raw = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool prealloc_ok = 0;
    double extracted_number = 0.0;
    cJSON_bool name_is_string = 0;
    cJSON_bool raws_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    value_num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", value_num);
    value_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", value_str);
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    dup_root = cJSON_Duplicate(root, 1);
    dup_value = cJSON_GetObjectItemCaseSensitive(dup_root, "value");
    extracted_number = cJSON_GetNumberValue(dup_value);
    dup_name = cJSON_GetObjectItemCaseSensitive(dup_root, "name");
    name_is_string = cJSON_IsString(dup_name);
    orig_raw = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    dup_raw = cJSON_GetObjectItemCaseSensitive(dup_root, "meta_raw");
    raws_equal = cJSON_Compare(orig_raw, dup_raw, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)prealloc_ok;
    (void)extracted_number;
    (void)name_is_string;
    (void)raws_equal;
    return 66;
}