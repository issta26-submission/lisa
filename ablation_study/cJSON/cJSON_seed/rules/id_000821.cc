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
//<ID> 821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *false_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_obj = NULL;
    cJSON *dup_num = NULL;
    cJSON *dup_str = NULL;
    char *printed = NULL;
    char buffer[256];
    double original_value = 0.0;
    double duplicate_value = 0.0;
    cJSON_bool greeting_is_string = 0;
    cJSON_bool dup_greeting_is_string = 0;
    cJSON_bool numbers_equal = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "object", obj);
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(obj, "value", num_item);
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(obj, "greeting", str_item);
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "raw_meta", raw_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    dup_obj = cJSON_GetObjectItem(dup_root, "object");
    dup_num = cJSON_GetObjectItem(dup_obj, "value");
    dup_str = cJSON_GetObjectItem(dup_obj, "greeting");
    original_value = cJSON_GetNumberValue(num_item);
    duplicate_value = cJSON_GetNumberValue(dup_num);
    numbers_equal = cJSON_Compare(num_item, dup_num, 1);
    greeting_is_string = cJSON_IsString(str_item);
    dup_greeting_is_string = cJSON_IsString(dup_str);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)original_value;
    (void)duplicate_value;
    (void)greeting_is_string;
    (void)dup_greeting_is_string;
    (void)numbers_equal;
    return 66;
}