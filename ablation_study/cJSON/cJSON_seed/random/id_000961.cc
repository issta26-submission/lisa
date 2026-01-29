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
//<ID> 961
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_key = "count";
    const char *str_key = "label";
    const char *bool_key = "valid";
    const char *str_val = "example";
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON *fetched_str_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object
    root = cJSON_CreateObject();

    // step 3: Configure - create/add number, bool and string (use required APIs)
    num_item = cJSON_AddNumberToObject(root, num_key, 7.0);
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);
    str_item = cJSON_CreateString(str_val);
    add_ok = cJSON_AddItemToObject(root, str_key, str_item);

    // step 4: Operate - retrieve the string item and extract its C string value
    fetched_str_item = cJSON_GetObjectItem(root, str_key);
    retrieved_str = cJSON_GetStringValue(fetched_str_item);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)add_ok + (int)(retrieved_str != (char *)0 && retrieved_str[0] == 'e');
    (void)validation_score;

    // step 6: Cleanup - delete root (which frees all attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}