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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *meta_flag_number = (cJSON *)0;
    char *printed_json = (char *)0;
    cJSON_bool meta_is_object = 0;

    // step 2: Initialize - create objects and attach a string
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(meta, "greeting", greeting);
    cJSON_AddFalseToObject(root, "active");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate - inspect meta and record result as a number in root
    meta_is_object = cJSON_IsObject(meta);
    meta_flag_number = cJSON_CreateNumber((double)meta_is_object);
    cJSON_AddItemToObject(root, "meta_is_object", meta_flag_number);

    // step 4: Validate - render JSON to a buffer and free the buffer
    printed_json = cJSON_Print(root);
    cJSON_free((void *)printed_json);

    // step 5: Cleanup - delete the whole tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}