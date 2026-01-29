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
//<ID> 1016
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"tester\",\"maybe_null\":null,\"count\":3}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *maybe_null_item = cJSON_GetObjectItem(root, "maybe_null");
    cJSON_bool was_null = cJSON_IsNull(maybe_null_item);
    cJSON_AddBoolToObject(root, "was_null", was_null);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("created_label");
    cJSON_AddItemToObject(new_obj, "label", label);
    cJSON_AddItemToObject(root, "extra", new_obj);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON_AddBoolToObject(root, "equal_dup", equal);
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}