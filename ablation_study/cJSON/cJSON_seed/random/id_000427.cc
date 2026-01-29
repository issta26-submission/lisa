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
//<ID> 427
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
    cJSON *got = (cJSON *)0;
    cJSON *btrue = (cJSON *)0;
    cJSON *bfalse = (cJSON *)0;
    cJSON *doub = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_ok = 0;
    double extracted = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(42.0);
    n1 = cJSON_CreateNumber(7.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    added_ok = cJSON_AddItemToObjectCS(root, "data", arr);
    btrue = cJSON_AddTrueToObject(root, "success");
    bfalse = cJSON_AddFalseToObject(root, "error");

    // step 4: Operate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    got = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got);
    doub = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToObjectCS(root, "double_second", doub);

    // step 5: Validate
    (void)added_ok;
    (void)btrue;
    (void)bfalse;
    (void)got;
    (void)extracted;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}