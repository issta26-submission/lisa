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
//<ID> 1801
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s_old = (cJSON *)0;
    cJSON *s_new = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    const char *strval = (const char *)0;
    int validation = 0;
    cJSON_bool ok_add1 = 0;
    cJSON_bool ok_add2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s_old = cJSON_CreateString("original");
    flag = cJSON_CreateFalse();

    // step 3: Configure
    ok_add1 = cJSON_AddItemToObject(root, "text", s_old);
    ok_add2 = cJSON_AddItemToObject(root, "flag", flag);
    s_new = cJSON_CreateString("replaced");

    // step 4: Operate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_ReplaceItemViaPointer(root, retrieved_flag, s_new);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    strval = cJSON_GetStringValue(retrieved_flag);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(s_old != (cJSON *)0) + (int)(flag != (cJSON *)0) + (int)(s_new != (cJSON *)0) + (int)(retrieved_flag != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(ok_add1 != 0) + (int)(ok_add2 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)s_old;
    (void)s_new;
    (void)flag;
    (void)retrieved_flag;
    (void)strval;
    (void)ok_add1;
    (void)ok_add2;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}