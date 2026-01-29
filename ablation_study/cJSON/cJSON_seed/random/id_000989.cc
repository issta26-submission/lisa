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
//<ID> 989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_str = "message";
    const char *key_num = "value";
    const char *text = "example";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON *fetched_str_item = (cJSON *)0;
    cJSON *fetched_num_item = (cJSON *)0;
    char *serialized = (char *)0;
    const char *retrieved_str = (const char *)0;
    double retrieved_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object and a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(text);

    // step 3: Configure - attach the string using AddItemToObject and add a number using AddNumberToObject
    add_ok = cJSON_AddItemToObject(root, key_str, str_item);
    num_item = cJSON_AddNumberToObject(root, key_num, 123.0);

    // step 4: Operate - serialize and then retrieve items using both case-insensitive and case-sensitive getters
    serialized = cJSON_Print(root);
    fetched_str_item = cJSON_GetObjectItem(root, key_str);
    fetched_num_item = cJSON_GetObjectItemCaseSensitive(root, key_num);
    retrieved_str = cJSON_GetStringValue(fetched_str_item);
    retrieved_num = cJSON_GetNumberValue(fetched_num_item);

    // step 5: Validate - compute a simple validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok) + (int)(num_item != (cJSON *)0) + (int)(serialized != (char *)0) + (int)(retrieved_str != (const char *)0 && retrieved_str[0] == 'e') + (int)(retrieved_num == 123.0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}