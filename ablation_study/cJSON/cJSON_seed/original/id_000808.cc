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
//<ID> 808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "flag");
    cJSON_AddNumberToObject(root, "value", 3.14);
    cJSON *list = cJSON_AddArrayToObject(root, "list");
    cJSON_AddItemToArray(list, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(list, cJSON_CreateString("two"));
    cJSON_AddItemToArray(list, cJSON_CreateFalse());
    cJSON_AddRawToObject(root, "meta", "{\"embedded\":true}");

    // step 2: Configure
    int list_size = cJSON_GetArraySize(list);
    cJSON_AddNumberToObject(root, "count", (double)list_size);
    cJSON *meta_item = cJSON_GetObjectItemCaseSensitive(root, "meta");
    const char *meta_raw = cJSON_GetStringValue(meta_item);
    cJSON_AddStringToObject(root, "meta_copy", meta_raw);

    // step 3: Operate
    cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON *dup_list = cJSON_Duplicate(list, 1);
    cJSON_bool lists_equal = cJSON_Compare(list, dup_list, 1);
    cJSON_Delete(dup_list);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)flag_is_true;
    (void)lists_equal;
    return 66;
}