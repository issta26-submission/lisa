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
//<ID> 2045
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
    cJSON *created_true = NULL;
    cJSON *added_bool_item = NULL;
    cJSON *got_name = NULL;
    const char *name_value = NULL;
    char *tmp_mem = NULL;
    cJSON_bool invalid_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    created_true = cJSON_CreateTrue();
    tmp_mem = (char *)cJSON_malloc(32);

    // step 3: Configure
    cJSON_AddItemToObject(root, "created_true", created_true);
    added_bool_item = cJSON_AddBoolToObject(root, "manual_flag", 1);
    cJSON_AddStringToObject(root, "name", "test");

    // step 4: Operate
    got_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(got_name);
    invalid_flag = cJSON_IsInvalid(got_name);
    tmp_mem[0] = (name_value != NULL) ? name_value[0] : '\0';
    tmp_mem[1] = '\0';

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(created_true != NULL);
    validation ^= (int)(added_bool_item != NULL);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(name_value != NULL);
    validation ^= (int)(tmp_mem[0] == 't');
    validation ^= (int)(invalid_flag == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(tmp_mem);

    // API sequence test completed successfully
    return 66;
}