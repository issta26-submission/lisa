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
//<ID> 1132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *fetched_greeting = (cJSON *)0;
    cJSON *fetched_list = (cJSON *)0;
    cJSON *fetched_first = (cJSON *)0;
    char *greeting_text = (char *)0;
    char *first_text = (char *)0;
    char *printed = (char *)0;
    cJSON_bool added_flag = (cJSON_bool)0;
    cJSON_bool is_greeting_str = (cJSON_bool)0;
    cJSON_bool is_first_str = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    greeting = cJSON_CreateString("hello world");
    elem0 = cJSON_CreateString("alpha");
    elem1 = cJSON_CreateString("beta");

    // step 3: Configure - attach items to object and populate array
    added_flag = cJSON_AddItemToObject(root, "greeting", greeting);
    added_flag = cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);

    // step 4: Operate - retrieve members and array elements, extract strings
    fetched_greeting = cJSON_GetObjectItem(root, "greeting");
    greeting_text = cJSON_GetStringValue(fetched_greeting);
    fetched_list = cJSON_GetObjectItem(root, "list");
    fetched_first = cJSON_GetArrayItem(fetched_list, 0);
    first_text = cJSON_GetStringValue(fetched_first);

    // step 5: Validate - inspect types and serialize then free serialized buffer
    is_greeting_str = cJSON_IsString(fetched_greeting);
    is_first_str = cJSON_IsString(fetched_first);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)is_greeting_str + (int)is_first_str + (int)(greeting_text != (char *)0) + (int)(first_text != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}