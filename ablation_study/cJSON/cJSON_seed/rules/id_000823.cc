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
//<ID> 823
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw_meta = NULL;
    cJSON *flag_false = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *picked_num = NULL;
    cJSON *picked_str = NULL;
    char *printed = NULL;
    double num_value = 0.0;
    cJSON_bool is_str = 0;
    cJSON_bool roots_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw_meta = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_meta);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    picked_num = cJSON_GetObjectItemCaseSensitive(root, "pi");
    picked_str = cJSON_GetObjectItemCaseSensitive(dup_root, "greeting");
    num_value = cJSON_GetNumberValue(picked_num);
    is_str = cJSON_IsString(picked_str);
    roots_equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)num_value;
    (void)is_str;
    (void)roots_equal;
    return 66;
}