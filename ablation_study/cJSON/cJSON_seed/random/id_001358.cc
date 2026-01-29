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
//<ID> 1358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *dup_child = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    const char *greeting = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, a child object, and a numeric item
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure - attach the number into the child and attach child and a string into root
    cJSON_AddItemToObject(child, "answer", num_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 4: Operate - duplicate the child (deep) and attach the duplicate to root, then retrieve the greeting string value
    dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    greeting = cJSON_GetStringValue(retrieved_greeting);

    // step 5: Validate - compute a simple validation score from observed values
    validation_score = (int)(root != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(dup_child != (cJSON *)0) + (int)(greeting != (const char *)0) + (int)(greeting[0] == 'h');

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)greeting;
    return 66;
}