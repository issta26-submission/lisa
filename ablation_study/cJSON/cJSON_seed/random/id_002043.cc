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
//<ID> 2043
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *created_true = NULL;
    cJSON *added_bool_item = NULL;
    char *got_str = NULL;
    void *mem_copy = NULL;
    size_t copy_len = 0;
    cJSON_bool invalid_flag = 0;
    cJSON_bool add_str_ok = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("cjson_demo");
    add_str_ok = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Configure
    created_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "created_true", created_true);
    added_bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    got_str = (char *)cJSON_GetStringValue(str_item);
    invalid_flag = cJSON_IsInvalid(created_true);
    copy_len = (got_str != NULL) ? (strlen(got_str) + 1) : 0;
    mem_copy = (copy_len > 0) ? cJSON_malloc(copy_len) : NULL;
    if (mem_copy != NULL) { memcpy(mem_copy, got_str, copy_len); }

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(created_true != NULL);
    validation ^= (int)(added_bool_item != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(mem_copy != NULL);
    validation ^= (int)(add_str_ok != 0);
    validation ^= (int)(invalid_flag == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    if (mem_copy) { cJSON_free(mem_copy); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}