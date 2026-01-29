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
//<ID> 827
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw = NULL;
    cJSON *flag_false = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *orig_meta = NULL;
    cJSON *dup_meta = NULL;
    char *printed = NULL;
    double num_value = 0.0;
    cJSON_bool is_str = 0;
    cJSON_bool metas_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "answer", num_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    num_item = cJSON_GetObjectItem(root, "answer");
    num_value = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "picked_value", num_value);
    str_item = cJSON_GetObjectItem(root, "greeting");
    is_str = cJSON_IsString(str_item);
    dup_root = cJSON_Duplicate(root, 1);
    orig_meta = cJSON_GetObjectItem(root, "meta_raw");
    dup_meta = cJSON_GetObjectItem(dup_root, "meta_raw");
    metas_equal = cJSON_Compare(orig_meta, dup_meta, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_str;
    (void)metas_equal;
    (void)num_value;
    return 66;
}