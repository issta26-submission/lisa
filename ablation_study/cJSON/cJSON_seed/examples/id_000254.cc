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
//<ID> 254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "alice", "bob", "carol" };
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON *value = cJSON_CreateNumber(123.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "names", names_arr);
    cJSON_AddItemToObjectCS(root, "value", value);

    // step 3: Operate & Validate
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON *has_num = cJSON_CreateNumber((double)has_names);
    cJSON_AddItemToObjectCS(root, "has_names", has_num);
    double has_val = cJSON_GetNumberValue(has_num);
    cJSON *has_copy = cJSON_CreateNumber(has_val);
    cJSON_AddItemToObjectCS(root, "has_names_copy", has_copy);
    cJSON *value_ptr = cJSON_GetObjectItem(root, "value");
    cJSON *replacement = cJSON_CreateNumber(999.0);
    cJSON_ReplaceItemViaPointer(root, value_ptr, replacement);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}