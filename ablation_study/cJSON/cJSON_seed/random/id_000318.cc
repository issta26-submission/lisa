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
//<ID> 318
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *got_s1 = (cJSON *)0;
    cJSON *got_flag = (cJSON *)0;
    const char *val1 = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool flag_is_true = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();

    // step 3: Configure
    s1 = cJSON_CreateString("hello");
    s2 = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "greeting1", s1);
    cJSON_AddItemToObject(root, "greeting2", s2);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddTrueToObject(nested, "active");

    // step 4: Operate
    got_s1 = cJSON_GetObjectItem(root, "greeting1");
    val1 = cJSON_GetStringValue(got_s1);
    got_flag = cJSON_GetObjectItem(nested, "active");
    flag_is_true = cJSON_IsTrue(got_flag);

    // step 5: Validate
    cJSON_AddStringToObject(root, "greeting1_copy", val1);
    cJSON_AddBoolToObject(root, "active_copy", flag_is_true);

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}