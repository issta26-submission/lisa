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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *raw_meta = NULL;
    cJSON *flag_false = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_nested = NULL;
    cJSON *dup_raw_meta = NULL;
    char *printed = NULL;
    char buffer[256];
    double number_value = 0.0;
    cJSON_bool is_string = 0;
    cJSON_bool metas_equal = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    raw_meta = cJSON_CreateRaw("{\"meta\":123}");
    cJSON_AddItemToObject(nested, "meta_raw", raw_meta);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(nested, "enabled", flag_false);
    num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(nested, "answer", num_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    number_value = cJSON_GetNumberValue(num_item);
    is_string = cJSON_IsString(str_item);
    dup_root = cJSON_Duplicate(root, 1);
    dup_nested = cJSON_GetObjectItemCaseSensitive(dup_root, "nested");
    dup_raw_meta = cJSON_GetObjectItemCaseSensitive(dup_nested, "meta_raw");
    metas_equal = cJSON_Compare(cJSON_GetObjectItemCaseSensitive(nested, "meta_raw"), dup_raw_meta, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)number_value;
    (void)is_string;
    (void)metas_equal;
    return 66;
}