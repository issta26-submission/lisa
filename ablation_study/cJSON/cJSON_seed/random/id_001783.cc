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
//<ID> 1783
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *detached_num = (cJSON *)0;
    cJSON *detached_str = (cJSON *)0;
    cJSON_bool add_ref_ok = 0;
    char *retrieved_cstr = (char *)0;
    double retrieved_num = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(123.456);
    str = cJSON_CreateString("hello world");

    // step 3: Configure
    cJSON_AddItemToObject(root, "value", num);
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "greeting_ref", str);

    // step 4: Operate
    detached_str = cJSON_DetachItemFromObjectCaseSensitive(root, "greeting_ref");
    detached_num = cJSON_DetachItemFromObjectCaseSensitive(root, "value");
    retrieved_cstr = cJSON_GetStringValue(detached_str);
    retrieved_num = cJSON_GetNumberValue(detached_num);

    // step 5: Validate
    validation = (int)(detached_str != (cJSON *)0) + (int)(detached_num != (cJSON *)0) + (int)(retrieved_cstr != (char *)0) + (int)(retrieved_num == 123.456) + (int)(add_ref_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached_str);
    cJSON_Delete(detached_num);
    cJSON_Delete(root);
    (void)root;
    (void)num;
    (void)str;
    (void)detached_num;
    (void)detached_str;
    (void)add_ref_ok;
    (void)retrieved_cstr;
    (void)retrieved_num;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}