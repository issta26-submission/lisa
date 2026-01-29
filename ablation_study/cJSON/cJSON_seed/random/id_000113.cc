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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *flag_num = (cJSON *)0;
    char *snapshot = (char *)0;
    cJSON_bool is_obj = 0;

    // step 2: Initialize - build object hierarchy and attach a string
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(inner, "greeting", greeting);
    cJSON_AddItemToObject(root, "inner", inner);

    // step 3: Operate - verify type and record it as a numeric flag
    is_obj = cJSON_IsObject(inner);
    flag_num = cJSON_CreateNumber((double)is_obj);
    cJSON_AddItemToObject(root, "inner_is_object", flag_num);

    // step 4: Configure additional state using specified helper
    cJSON_AddFalseToObject(root, "explicit_false");

    // step 5: Validate - produce a compact snapshot and free the buffer via cJSON_free
    snapshot = cJSON_PrintUnformatted(root);
    cJSON_free((void *)snapshot);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}