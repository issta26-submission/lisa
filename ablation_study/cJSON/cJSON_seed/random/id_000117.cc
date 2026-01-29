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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s_name = (cJSON *)0;
    cJSON *s_greet = (cJSON *)0;
    cJSON_bool root_is_object = 0;
    void *temp_buffer = (void *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s_name = cJSON_CreateString("alice");
    s_greet = cJSON_CreateString("hello, alice");
    cJSON_AddItemToObject(root, "name", s_name);
    cJSON_AddItemToObject(root, "greeting", s_greet);
    cJSON_AddFalseToObject(root, "subscribed");

    // step 3: Operate
    root_is_object = cJSON_IsObject(root);
    temp_buffer = cJSON_malloc(64);

    // step 4: Validate
    (void)root_is_object;
    (void)temp_buffer;

    // step 5: Cleanup
    cJSON_free(temp_buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}