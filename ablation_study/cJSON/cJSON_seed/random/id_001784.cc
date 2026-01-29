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
//<ID> 1784
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_str_obj = (cJSON *)0;
    cJSON *retrieved_num_obj = (cJSON *)0;
    cJSON *detached_str = (cJSON *)0;
    cJSON *detached_num = (cJSON *)0;
    char *strval = (char *)0;
    double numval = 0.0;
    cJSON_bool add_ref_ok = 0;
    cJSON_bool add_num_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str = cJSON_CreateString("hello world");
    num = cJSON_CreateNumber(123.0);

    // step 3: Configure
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "greeting", str);
    add_num_ok = cJSON_AddItemToObject(root, "answer", num);

    // step 4: Operate
    retrieved_str_obj = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    strval = cJSON_GetStringValue(retrieved_str_obj);
    retrieved_num_obj = cJSON_GetObjectItem(root, "answer");
    numval = cJSON_GetNumberValue(retrieved_num_obj);

    // step 5: Validate
    validation = (int)(strval != (char *)0) + (int)(numval == 123.0) + (int)(add_ref_ok != 0) + (int)(add_num_ok != 0);
    (void)validation;

    // step 6: Cleanup
    detached_str = cJSON_DetachItemFromObjectCaseSensitive(root, "greeting");
    detached_num = cJSON_DetachItemFromObjectCaseSensitive(root, "answer");
    cJSON_Delete(detached_str);
    cJSON_Delete(detached_num);
    cJSON_Delete(root);
    (void)root;
    (void)str;
    (void)num;
    (void)retrieved_str_obj;
    (void)retrieved_num_obj;
    (void)strval;
    (void)numval;
    (void)add_ref_ok;
    (void)add_num_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}