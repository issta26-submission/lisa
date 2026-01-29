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
//<ID> 721
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *pi_number = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    char *greeting_value = (char *)0;
    cJSON *copied_greeting = (cJSON *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    flag_false = cJSON_CreateFalse();
    pi_number = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON_AddItemToObject(root, "pi", pi_number);

    // step 4: Operate
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    greeting_value = cJSON_GetStringValue(retrieved_greeting);
    copied_greeting = cJSON_CreateString(greeting_value);
    cJSON_AddItemToObject(root, "greeting_copy", copied_greeting);

    // step 5: Validate
    validation_score = (int)(greeting_value != (char *)0)
                     + (int)cJSON_IsString(retrieved_greeting)
                     + (int)(cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi")) != 0.0);

    // step 6: Cleanup
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}