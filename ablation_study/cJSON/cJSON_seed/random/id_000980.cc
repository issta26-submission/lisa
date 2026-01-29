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
//<ID> 980
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_name = "name";
    const char *key_score = "score";
    const char *person = "Alice";
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *score_item = (cJSON *)0;
    cJSON_bool add_ok_name = (cJSON_bool)0;
    cJSON *fetched_score = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString(person);

    // step 3: Configure - attach the string via AddItemToObject and add a number via AddNumberToObject
    add_ok_name = cJSON_AddItemToObject(root, key_name, name_item);
    score_item = cJSON_AddNumberToObject(root, key_score, 99.5);

    // step 4: Operate - retrieve items using both case-insensitive and case-sensitive getters and serialize
    fetched_score = cJSON_GetObjectItem(root, key_score);
    fetched_name = cJSON_GetObjectItemCaseSensitive(root, key_name);
    json_str = cJSON_Print(root);

    // step 5: Validate - compute a small validation score exercising the results
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(add_ok_name)
                     + (int)(score_item != (cJSON *)0)
                     + (int)(fetched_score == score_item)
                     + (int)(fetched_name == name_item)
                     + (int)(json_str != (char *)0 && json_str[0] == '{');
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}