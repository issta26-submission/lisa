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
//<ID> 1782
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *node = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_node = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    cJSON *detached_num = (cJSON *)0;
    cJSON *detached_node = (cJSON *)0;
    char *str_val = (char *)0;
    double num_val = 0.0;
    int validation = 0;
    cJSON_bool add_ref_ok = 0;
    cJSON_bool add_node_ok = 0;
    cJSON_bool add_num_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    node = cJSON_CreateObject();
    name_str = cJSON_CreateString("referenced-name");
    num = cJSON_CreateNumber(123.456);

    // step 3: Configure
    add_ref_ok = cJSON_AddItemReferenceToObject(node, "label", name_str);
    add_node_ok = cJSON_AddItemToObject(root, "node", node);
    add_num_ok = cJSON_AddItemToObject(root, "value", num);

    // step 4: Operate
    retrieved_node = cJSON_GetObjectItem(root, "node");
    ref_item = cJSON_GetObjectItem(retrieved_node, "label");
    str_val = cJSON_GetStringValue(ref_item);
    detached_num = cJSON_DetachItemFromObjectCaseSensitive(root, "value");
    num_val = cJSON_GetNumberValue(detached_num);

    // step 5: Validate
    validation = (int)(retrieved_node != (cJSON *)0) + (int)(ref_item != (cJSON *)0) + (int)(str_val != (char *)0) + (int)(detached_num != (cJSON *)0) + (int)(num_val == 123.456) + (int)(add_ref_ok != 0) + (int)(add_node_ok != 0) + (int)(add_num_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached_num);
    detached_node = cJSON_DetachItemFromObjectCaseSensitive(root, "node");
    cJSON_Delete(detached_node);
    cJSON_Delete(root);
    (void)root;
    (void)node;
    (void)name_str;
    (void)num;
    (void)retrieved_node;
    (void)ref_item;
    (void)detached_num;
    (void)detached_node;
    (void)str_val;
    (void)num_val;
    (void)add_ref_ok;
    (void)add_node_ok;
    (void)add_num_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}