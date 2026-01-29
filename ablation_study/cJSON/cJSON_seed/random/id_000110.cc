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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    char *json_str = (char *)0;
    cJSON_bool is_obj = 0;
    double obj_flag = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s1 = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", s1);

    // step 3: Configure
    cJSON_AddFalseToObject(root, "active");
    s2 = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "target", s2);

    // step 4: Operate / Validate
    is_obj = cJSON_IsObject(root);
    obj_flag = (double)is_obj;
    cJSON_AddNumberToObject(root, "is_object", obj_flag);
    json_str = cJSON_PrintUnformatted(root);

    // step 5: Cleanup
    cJSON_free((void *)json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}