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
//<ID> 644
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzz_test");
    cJSON *desc_item = cJSON_CreateString("a simple cJSON usage test");
    cJSON_AddItemToObject(meta, "desc", desc_item);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(list, n1);

    // step 2: Configure
    cJSON_AddStringToObject(root, "version", cJSON_Version());
    double prev_value = cJSON_SetNumberHelper(n1, 6.28);
    cJSON_AddNumberToObject(root, "previous_value", prev_value);

    // step 3: Operate and Validate
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(meta, "name_read", name_value);
    cJSON_bool is_false = cJSON_IsFalse(flag_false);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)is_false);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    cJSON_AddStringToObject(root, "snapshot", buffer);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}