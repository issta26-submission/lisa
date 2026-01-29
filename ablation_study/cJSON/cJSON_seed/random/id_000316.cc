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
//<ID> 316
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *t = (cJSON *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    s1 = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(child, "greeting", s1);
    t = cJSON_AddTrueToObject(child, "enabled");
    s2 = cJSON_CreateString("root level note");
    cJSON_AddItemToObject(root, "meta", s2);
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("configured"));

    // step 5: Validate
    cJSON_AddItemToObject(root, "final_check", cJSON_CreateString("all_set"));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}