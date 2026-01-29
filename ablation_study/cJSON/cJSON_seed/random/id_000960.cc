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
//<ID> 960
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_str = "message";
    const char *key_num = "count";
    const char *key_flag = "active";
    const char *initial_str = "test message";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON *fetched_str_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(initial_str);

    // step 3: Configure - attach items using required APIs
    add_ok = cJSON_AddItemToObject(root, key_str, str_item);
    num_item = cJSON_AddNumberToObject(root, key_num, 123.0);
    bool_item = cJSON_AddBoolToObject(root, key_flag, (cJSON_bool)1);

    // step 4: Operate - retrieve the string item and extract its C string
    fetched_str_item = cJSON_GetObjectItem(root, key_str);
    retrieved_str = cJSON_GetStringValue(fetched_str_item);

    // step 5: Validate - compute a simple validation score using results
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok + (int)(num_item != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(retrieved_str != (char *)0 && retrieved_str[0] == 't');
    (void)validation_score;

    // step 6: Cleanup - free the whole tree (root owns attached items)
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}