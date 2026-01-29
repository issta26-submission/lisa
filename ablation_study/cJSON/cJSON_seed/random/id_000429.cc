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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *doubled = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    cJSON *flag_true = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    char *printed = (char *)0;
    double val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(42.0);
    n1 = cJSON_CreateNumber(3.14);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObjectCS(root, "values", arr);
    flag_true = cJSON_AddTrueToObject(root, "is_ok");
    flag_false = cJSON_AddFalseToObject(root, "is_bad");

    // step 4: Operate
    got1 = cJSON_GetArrayItem(arr, 1);
    val = cJSON_GetNumberValue(got1);
    doubled = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObjectCS(root, "doubled", doubled);

    // step 5: Validate
    (void)flag_true;
    (void)flag_false;
    (void)val;
    (void)got1;

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}