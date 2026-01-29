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
//<ID> 1780
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *srcObj = (cJSON *)0;
    cJSON *greet = (cJSON *)0;
    cJSON *ans = (cJSON *)0;
    cJSON *detached_ref = (cJSON *)0;
    cJSON *retrieved_greet = (cJSON *)0;
    cJSON *retrieved_ans = (cJSON *)0;
    const char *greet_val = (const char *)0;
    double ans_val = 0.0;
    cJSON_bool add_ref_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    srcObj = cJSON_CreateObject();
    greet = cJSON_CreateString("hello world");
    ans = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(srcObj, "greeting", greet);
    cJSON_AddItemToObject(srcObj, "answer", ans);

    // step 3: Configure
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "alias", srcObj);

    // step 4: Operate
    detached_ref = cJSON_DetachItemFromObjectCaseSensitive(root, "alias");
    retrieved_greet = cJSON_GetObjectItem(detached_ref, "greeting");
    retrieved_ans = cJSON_GetObjectItem(detached_ref, "answer");
    greet_val = cJSON_GetStringValue(retrieved_greet);
    ans_val = cJSON_GetNumberValue(retrieved_ans);

    // step 5: Validate
    validation = (int)(add_ref_ok != 0) + (int)(detached_ref != (cJSON *)0) + (int)(retrieved_greet != (cJSON *)0) + (int)(retrieved_ans != (cJSON *)0) + (int)(greet_val != (const char *)0) + (int)(ans_val == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(srcObj);
    (void)detached_ref;
    (void)retrieved_greet;
    (void)retrieved_ans;
    (void)greet_val;
    (void)ans_val;
    (void)add_ref_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}