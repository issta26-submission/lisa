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
//<ID> 1785
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *alias = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *detached_value_item = (cJSON *)0;
    cJSON_bool add_ok = 0;
    const char *label_str = (const char *)0;
    double value_num = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("answer");

    // step 3: Configure
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(child, "label", str_item);
    add_ok = cJSON_AddItemReferenceToObject(root, "alias", child);

    // step 4: Operate
    alias = cJSON_GetObjectItemCaseSensitive(root, "alias");
    label_item = cJSON_GetObjectItemCaseSensitive(alias, "label");
    label_str = cJSON_GetStringValue(label_item);
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "alias");
    detached_value_item = cJSON_GetObjectItemCaseSensitive(detached, "value");
    value_num = cJSON_GetNumberValue(detached_value_item);

    // step 5: Validate
    validation = (int)(add_ok != 0) + (int)(alias != (cJSON *)0) + (int)(label_item != (cJSON *)0) + (int)(label_str != (const char *)0) + (int)(detached != (cJSON *)0) + (int)(detached_value_item != (cJSON *)0) + (int)(value_num == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)root;
    (void)child;
    (void)num_item;
    (void)str_item;
    (void)alias;
    (void)label_item;
    (void)detached;
    (void)detached_value_item;
    (void)add_ok;
    (void)label_str;
    (void)value_num;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}