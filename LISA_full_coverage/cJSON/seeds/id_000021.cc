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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"alpha\": 10, \"text\": \"orig\"}";
    size_t len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, len);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 42.5);
    cJSON_AddItemToObject(root, "num", num_item);

    cJSON *str_item = cJSON_CreateString("placeholder");
    char *old_str = cJSON_SetValuestring(str_item, "updated");
    (void)old_str;
    cJSON_AddItemToObject(root, "str", str_item);

    const char raw[] = "{\"raw_key\": 99}";
    cJSON_AddRawToObject(root, "injected", raw);

    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_dup", dup);

    cJSON *ref = cJSON_CreateObjectReference(dup);
    cJSON_AddItemToObject(root, "dup_ref", ref);

    // step 3: Operate and Validate
    cJSON *got_num = cJSON_GetObjectItem(root, "num");
    double val = cJSON_GetNumberValue(got_num);

    cJSON *got_str = cJSON_GetObjectItem(root, "str");
    const char *sval = cJSON_GetStringValue(got_str);

    cJSON_bool equal = cJSON_Compare(dup, parsed, 0);

    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    (void)val;
    (void)sval;
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}