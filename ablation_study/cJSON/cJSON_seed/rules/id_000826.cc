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
//<ID> 826
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
    cJSON *dup_meta = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    double pi_value = 0.0;
    cJSON_bool is_str = 0;
    cJSON_bool prealloc_ok = 0;
    cJSON_bool metas_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", num_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    dup_root = cJSON_Duplicate(root, 1);
    pi_value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "pi"));
    is_str = cJSON_IsString(cJSON_GetObjectItem(root, "greeting"));
    dup_meta = cJSON_GetObjectItemCaseSensitive(dup_root, "meta_raw");
    metas_equal = cJSON_Compare(raw, dup_meta, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)buffer;
    (void)pi_value;
    (void)is_str;
    (void)prealloc_ok;
    (void)metas_equal;
    return 66;
}