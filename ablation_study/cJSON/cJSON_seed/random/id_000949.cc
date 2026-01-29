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
//<ID> 949
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
    const char *str_val = "hello world";
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON *fetched_num_item = (cJSON *)0;
    cJSON *fetched_str_item = (cJSON *)0;
    double retrieved_num = 0.0;
    char *retrieved_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.5);
    str_item = cJSON_CreateString(str_val);

    // step 3: Configure - attach items (use both AddItemToObject and AddItemToObjectCS)
    add_ok1 = cJSON_AddItemToObject(root, num_key, num_item);
    add_ok2 = cJSON_AddItemToObjectCS(root, str_key, str_item);

    // step 4: Operate - retrieve items and extract values
    fetched_num_item = cJSON_GetObjectItem(root, num_key);
    fetched_str_item = cJSON_GetObjectItem(root, str_key);
    retrieved_num = cJSON_GetNumberValue(fetched_num_item);
    retrieved_str = cJSON_GetStringValue(fetched_str_item);

    // step 5: Validate - compute a simple score from retrieved values
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(retrieved_num == 42.5) + (int)(retrieved_str != (char *)0 && retrieved_str[0] == 'h');
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}