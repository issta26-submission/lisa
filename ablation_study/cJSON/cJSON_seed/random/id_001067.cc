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
//<ID> 1067
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *greeting_text = "hello-world";
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_child = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    char *printed = (char *)0;
    const char *got_string = (const char *)0;
    double got_number = 0.0;
    cJSON_bool add_ok_child_str = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    cJSON_bool add_ok_child = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, a child object, a number and a string item
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    strItem = cJSON_CreateString(greeting_text);

    // step 3: Configure - attach the string into the child using case-sensitive add, attach number and child into root using non-CS add
    add_ok_child_str = cJSON_AddItemToObjectCS(child, "greet", strItem);
    add_ok_num = cJSON_AddItemToObject(root, "answer", num);
    add_ok_child = cJSON_AddItemToObject(root, "meta", child);

    // step 4: Operate - fetch the number and string items, read their values, print and free the JSON text
    fetched_num = cJSON_GetObjectItem(root, "answer");
    got_number = cJSON_GetNumberValue(fetched_num);
    fetched_child = cJSON_GetObjectItem(root, "meta");
    fetched_str = cJSON_GetObjectItem(fetched_child, "greet");
    got_string = cJSON_GetStringValue(fetched_str);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score using the results
    validation_score = (int)(root != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(num != (cJSON *)0) + (int)(strItem != (cJSON *)0) + (int)(add_ok_child_str) + (int)(add_ok_num) + (int)(add_ok_child) + (int)(got_number == 42.0) + (int)(got_string != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}