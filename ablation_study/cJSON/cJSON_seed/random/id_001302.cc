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
//<ID> 1302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *greet = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_greet = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    int list_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and items
    root = cJSON_CreateObject();
    list = cJSON_CreateArray();
    greet = cJSON_CreateString("hello");
    flag_item = cJSON_CreateFalse();

    // step 3: Configure - attach items to the root object and populate the array
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON_AddItemToObject(root, "flag", flag_item);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddStringToObject(root, "summary", "completed");
    cJSON_AddItemToArray(list, cJSON_CreateString("one"));
    cJSON_AddItemToArray(list, cJSON_CreateString("two"));

    // step 4: Operate - retrieve items case-sensitively and inspect types/sizes
    retrieved_greet = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    list_size = cJSON_GetArraySize(list);
    flag_is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_greet != (cJSON *)0) + (int)(list_size == 2) + (int)flag_is_false;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}