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
//<ID> 1786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_num_item = (cJSON *)0;
    cJSON *retrieved_str_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    double retrieved_num_value = 0.0;
    cJSON_bool add_ref_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    str_item = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToObject(obj, "num", num_item);
    cJSON_AddItemToObject(obj, "text", str_item);
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "alias", obj);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "alias");
    retrieved_num_item = cJSON_GetObjectItem(detached, "num");
    retrieved_str_item = cJSON_GetObjectItem(detached, "text");
    retrieved_str = cJSON_GetStringValue(retrieved_str_item);
    retrieved_num_value = cJSON_GetNumberValue(retrieved_num_item);

    // step 5: Validate
    validation = (int)(detached != (cJSON *)0) + (int)(retrieved_str != (char *)0) + (int)(retrieved_num_value == 123.456) + (int)(add_ref_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
    (void)root;
    (void)obj;
    (void)num_item;
    (void)str_item;
    (void)retrieved_num_item;
    (void)retrieved_str_item;
    (void)retrieved_str;
    (void)retrieved_num_value;
    (void)add_ref_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}