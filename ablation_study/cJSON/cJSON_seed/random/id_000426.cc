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
//<ID> 426
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *calc = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    char *printed = (char *)0;
    double extracted = 0.0;
    cJSON_bool added_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(3.14);
    meta = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added_ok = cJSON_AddItemToObjectCS(root, "values", arr);
    cJSON_AddTrueToObject(root, "ok");
    cJSON_AddFalseToObject(root, "disabled");
    added_ok = cJSON_AddItemToObjectCS(meta, "greeting", greeting);
    added_ok = cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got_item);
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    calc = cJSON_CreateNumber(extracted * 2.0);
    added_ok = cJSON_AddItemToObjectCS(root, "calc", calc);
    (void)added_ok;
    (void)extracted;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}