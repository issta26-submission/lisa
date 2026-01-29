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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddNumberToObject(meta, "id", 42.0);
    cJSON *active_item = cJSON_AddFalseToObject(root, "active");
    cJSON_AddNumberToObject(root, "root_version", 1.0);

    // step 2: Configure
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *details = cJSON_AddObjectToObject(meta, "details");
    cJSON_AddNumberToObject(details, "score", 99.5);

    // step 3: Operate and Validate
    double is_false_flag = (double)cJSON_IsFalse(active_item);
    cJSON_AddNumberToObject(root, "active_flag", is_false_flag);
    double first_char_code = (double)(unsigned char)name_str[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "backup_meta", dup_meta);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}