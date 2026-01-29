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
//<ID> 888
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *greeting = "Hello, cJSON!";
    cJSON *root = (cJSON *)0;
    cJSON *greeting_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *number_item = (cJSON *)0;
    cJSON_bool added_greeting = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize root object and create string item
    root = cJSON_CreateObject();
    greeting_item = cJSON_CreateString(greeting);

    // step 3: Configure object by adding items
    added_greeting = cJSON_AddItemToObject(root, "greeting", greeting_item);
    true_item = cJSON_AddTrueToObject(root, "is_valid");
    number_item = cJSON_AddNumberToObject(root, "value", 123.456);

    // step 4: Validate by printing and aggregating simple checks
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(greeting_item != (cJSON *)0) + (int)added_greeting + (int)(true_item != (cJSON *)0) + (int)(number_item != (cJSON *)0) + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}