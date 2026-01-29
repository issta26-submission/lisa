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
//<ID> 1763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *answer = (cJSON *)0;
    cJSON *retrieved_nested = (cJSON *)0;
    cJSON_bool is_array = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    double answer_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    answer = cJSON_CreateNumber(42.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "nested", nested);
    add_ok2 = cJSON_AddItemToObject(nested, "greeting", greeting);
    add_ok3 = cJSON_AddItemToObject(nested, "answer", answer);

    // step 4: Operate
    retrieved_nested = cJSON_GetObjectItem(root, "nested");
    is_array = cJSON_IsArray(retrieved_nested);
    answer_value = cJSON_GetNumberValue(cJSON_GetObjectItem(retrieved_nested, "answer"));
    cJSON_DeleteItemFromObject(retrieved_nested, "greeting");

    // step 5: Validate
    validation = (int)(is_array == 0) + (int)(answer_value == 42.0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)nested;
    (void)greeting;
    (void)answer;
    (void)retrieved_nested;
    (void)is_array;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)answer_value;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}