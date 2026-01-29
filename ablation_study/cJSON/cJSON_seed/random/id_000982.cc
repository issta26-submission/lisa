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
//<ID> 982
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_greeting = "greeting";
    const char *key_nested = "nested";
    const char *key_answer = "answer";
    const char *greet_text = "example";
    cJSON *root = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_greeting = (cJSON *)0;
    cJSON *fetched_answer = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    char *json_str = (char *)0;
    const char *retrieved_greeting = (const char *)0;
    double retrieved_number = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root and nested object, create a string item
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    str_item = cJSON_CreateString(greet_text);

    // step 3: Configure - attach items to the root and add a number to the nested object
    add_ok = cJSON_AddItemToObject(root, key_greeting, str_item);
    cJSON_AddItemToObject(root, key_nested, nested);
    num_item = cJSON_AddNumberToObject(nested, key_answer, 42.0);

    // step 4: Operate - serialize and retrieve items using both retrieval APIs
    json_str = cJSON_Print(root);
    fetched_greeting = cJSON_GetObjectItem(root, key_greeting);
    fetched_answer = cJSON_GetObjectItemCaseSensitive(nested, key_answer);
    retrieved_greeting = cJSON_GetStringValue(fetched_greeting);
    retrieved_number = cJSON_GetNumberValue(fetched_answer);

    // step 5: Validate - compute a simple validation score exercising results
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok) + (int)(num_item != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(retrieved_greeting != (const char *)0 && retrieved_greeting[0] == 'e') + (int)(retrieved_number == 42.0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}