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
//<ID> 1443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *str_ref = cJSON_CreateStringReference("ref_string");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *dup_str = cJSON_Duplicate(str_ref, 1);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "nullval", null_item);
    cJSON_AddItemToObjectCS(root, "strref", str_ref);
    cJSON_AddItemToObjectCS(root, "flag", false_item);
    cJSON_AddItemToObjectCS(root, "dupref", dup_str);

    // step 3: Operate & Validate
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    cJSON_bool was_null = cJSON_IsNull(null_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_bool equal_refs = cJSON_Compare(dup_str, str_ref, 1);
    (void)was_false;
    (void)was_null;
    (void)equal_refs;
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}