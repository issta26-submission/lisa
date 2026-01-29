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
//<ID> 822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *raw = NULL;
    cJSON *false_item = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char buffer[256];
    double extracted_value = 0.0;
    cJSON_bool is_string = 0;
    cJSON_bool equal = 0;
    cJSON_bool printed_ok = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str);
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Operate / Validate
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    is_string = cJSON_IsString(str);
    extracted_value = cJSON_GetNumberValue(num);
    equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)printed_ok;
    (void)is_string;
    (void)extracted_value;
    (void)equal;
    return 66;
}