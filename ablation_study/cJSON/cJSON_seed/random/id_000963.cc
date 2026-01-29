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
//<ID> 963
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *str_key = "name";
    const char *num_key = "age";
    const char *bool_key = "active";
    const char *name_val = "Alice";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *fetched_str_item = (cJSON *)0;
    cJSON *fetched_num_item = (cJSON *)0;
    cJSON *num_added = (cJSON *)0;
    cJSON *bool_added = (cJSON *)0;
    cJSON_bool add_ok_item = (cJSON_bool)0;
    char *retrieved_str = (char *)0;
    double retrieved_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root and a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(name_val);

    // step 3: Configure - attach items using specified APIs
    add_ok_item = cJSON_AddItemToObject(root, str_key, str_item);
    num_added = cJSON_AddNumberToObject(root, num_key, 30.0);
    bool_added = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);

    // step 4: Operate - retrieve items and extract values
    fetched_str_item = cJSON_GetObjectItem(root, str_key);
    fetched_num_item = cJSON_GetObjectItem(root, num_key);
    retrieved_str = cJSON_GetStringValue(fetched_str_item);
    retrieved_num = cJSON_GetNumberValue(fetched_num_item);

    // step 5: Validate - compute a simple validation score from retrieved values
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok_item + (int)(num_added != (cJSON *)0) + (int)(bool_added != (cJSON *)0) + (int)(retrieved_str != (char *)0 && retrieved_str[0] == 'A') + (int)(retrieved_num == 30.0);
    (void)validation_score;

    // step 6: Cleanup - delete root (which frees children)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}