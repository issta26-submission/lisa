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
//<ID> 2048
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
    cJSON *bool_item_from_add = NULL;
    cJSON_bool added_msg = 0;
    cJSON_bool added_true_node = 0;
    void *alloc_buf = NULL;
    const char *retrieved = NULL;
    cJSON_bool invalid_check = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    alloc_buf = cJSON_malloc(32);
    memset(alloc_buf, 0, 32);
    ((char *)alloc_buf)[0] = 'h';
    ((char *)alloc_buf)[1] = 'i';
    ((char *)alloc_buf)[2] = '\0';
    str_item = cJSON_CreateString((const char *)alloc_buf);

    // step 3: Configure
    bool_item_from_add = cJSON_AddBoolToObject(root, "direct_bool", 1);
    cJSON *true_node = cJSON_CreateTrue();
    added_true_node = cJSON_AddItemToObject(root, "true_node", true_node);
    added_msg = cJSON_AddItemToObject(root, "msg", str_item);

    // step 4: Operate
    cJSON *got_msg = cJSON_GetObjectItem(root, "msg");
    retrieved = cJSON_GetStringValue(got_msg);
    invalid_check = cJSON_IsInvalid(bool_item_from_add);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item_from_add != NULL);
    validation ^= (int)(added_msg != 0);
    validation ^= (int)(added_true_node != 0);
    validation ^= (int)(retrieved != NULL);
    validation ^= (int)(invalid_check == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(alloc_buf);

    // API sequence test completed successfully
    return 66;
}