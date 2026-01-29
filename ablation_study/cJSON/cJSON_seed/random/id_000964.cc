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
//<ID> 964
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_key = "answer";
    const char *str_key = "greeting";
    const char *bool_key = "flag";
    const char *str_val = "hello world";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON *fetched_str_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(str_val);

    // step 3: Configure - attach items using different add APIs
    add_ok_str = cJSON_AddItemToObject(root, str_key, str_item);
    num_item = cJSON_AddNumberToObject(root, num_key, 123.0);
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);

    // step 4: Operate - retrieve the string item and get its C string value
    fetched_str_item = cJSON_GetObjectItem(root, str_key);
    retrieved_str = cJSON_GetStringValue(fetched_str_item);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok_str + (int)(num_item != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(retrieved_str != (char *)0 && retrieved_str[0] == 'h');
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}