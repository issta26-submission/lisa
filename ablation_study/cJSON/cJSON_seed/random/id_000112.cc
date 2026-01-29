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
//<ID> 112
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
    cJSON *flag = (cJSON *)0;
    cJSON_bool is_object = 0;
    char *unformatted = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s1 = cJSON_CreateString("hello");
    s2 = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "greeting", s1);
    cJSON_AddItemToObject(root, "target", s2);

    // step 3: Configure
    flag = cJSON_AddFalseToObject(root, "enabled");

    // step 4: Operate
    is_object = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "is_object", (double)is_object);
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    cJSON_free((void *)unformatted);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}