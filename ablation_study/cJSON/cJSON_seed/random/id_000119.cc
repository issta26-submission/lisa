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
//<ID> 119
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
    cJSON *s_desc = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON_bool is_obj = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s_name = cJSON_CreateString("example");
    s_desc = cJSON_CreateString("a sample object");

    // step 3: Configure - attach strings and boolean
    cJSON_AddItemToObject(root, "name", s_name);
    cJSON_AddItemToObject(root, "description", s_desc);
    flag = cJSON_AddFalseToObject(root, "enabled");

    // step 4: Operate & Validate - verify root is object and produce JSON text, then free it
    is_obj = cJSON_IsObject(root);
    json_text = cJSON_Print(root);
    cJSON_free(json_text);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}