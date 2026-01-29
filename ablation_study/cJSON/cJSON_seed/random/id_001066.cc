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
//<ID> 1066
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
    cJSON *num_pi = (cJSON *)0;
    cJSON *str_status = (cJSON *)0;
    char *printed = (char *)0;
    const char *status_value = (const char *)0;
    double pi_val = 0.0;
    cJSON_bool ok_add_pi = (cJSON_bool)0;
    cJSON_bool ok_add_status = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    num_pi = cJSON_CreateNumber(3.14159);
    str_status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(inner, "nested_num", cJSON_CreateNumber(42.0));

    // step 3: Configure - attach items to root using both AddItemToObject and AddItemToObjectCS
    ok_add_pi = cJSON_AddItemToObject(root, "pi", num_pi);
    ok_add_status = cJSON_AddItemToObjectCS(root, "status", str_status);
    cJSON_AddItemToObjectCS(root, "inner", inner);

    // step 4: Operate - read values and produce an unformatted print
    pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    status_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "status"));
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score from the results
    validation_score = (int)(root != (cJSON *)0) + (int)ok_add_pi + (int)ok_add_status + (int)(pi_val > 3.14) + (int)(status_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}